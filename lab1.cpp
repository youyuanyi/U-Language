#include<iostream>
#include<cstring>
#include<string>
#include<cstdlib>
#include<math.h>
#define ST_Length 100 //符号表长度
#define Mem_Length 100 //内存表长度
using namespace std;

string temp;//临时变量
//因为编译器版本问题，编译时要加上-fpermissive

//符号表
typedef struct {
    string Var_name; //变量名
    int     address;    //变量地址
    string Var_type;   //变量类型
}ST_Table[ST_Length];

//内存表
typedef struct {
    int flag;   //是否被使用
    string content; //内容
}Memory[Mem_Length];

void Init_ST(ST_Table* ST);//初始化符号表
void Init_Mem(Memory* M);//初始化内存表
int str_match(string str, char c);    //字符串匹配，遇到要找的字符，则返回字符位置i，否则返回-1
int Variable_Find(ST_Table* ST, string var_name);//查找符号表中的变量，若存在则返回符号表下标，不存在则返回-1
void Declare_Variable(string type, ST_Table* ST, Memory* Mem);  //声明各类型变量
void Judge_Var_Repeat(ST_Table* ST, Memory* Mem, string var_type, string var); //对变量名重复声明时的判断
void Declare_Write(ST_Table* ST, Memory* MEM, string var_type, string var); //声明变量时将信息写入ST表
void Write_ST(ST_Table* ST, string var_name, int address, string type); //将声明的变量及其信息写入符号表ST
int Calculate_Address(Memory* Mem);    //给内存地址赋值时，计算变量声明时应该写入模拟内存的首地址
void Variable_Assignment(ST_Table* ST, Memory* Mem, string  cmd_name);//给ST表声明的变量赋值并写入内存
void Print_ST(ST_Table* ST);//打印ST
void Print_Mem(Memory* Mem); //打印Mem
void Get_Address(ST_Table* ST, string cmd);//取得变量的地址
string Get_Value(ST_Table* ST, Memory* Mem, string cmd);//取得变量的值
void Variable_Add(ST_Table* ST, Memory* Mem, string cmd);//int或string的加法操作
string Get_Type(ST_Table* ST, string var);//获得var的类型
int Get_String_Len(ST_Table* ST, Memory* Mem, string var); //获取string的长度
void Int_OtherOperate(ST_Table* ST, Memory* Mem, string cmd);//整数的减法、乘法、除法操作
void String_Compare(ST_Table* ST, Memory* Mem, string cmd);
void Fun_Define(ST_Table* ST, Memory* Mem, string cmd);//函数定义函数

/*
初始化ST表
*/
void Init_ST(ST_Table* ST)
{
    for (int i = 0; i < ST_Length; i++) {
        ST[i]->address = 0;
    }
}

/*
初始化Mem*/
void Init_Mem(Memory* M)
{
    for (int i = 0; i < Mem_Length; i++) {
        M[i]->flag = 0;
    }
}

/*
字符匹配
*/
int str_match(string str, char c) {
    for (int i = 0; i < str.length(); ++i)
        if (str[i] == c)
            return i;
    return -1;
}


/*
在st表中寻找变量
*/
int Variable_Find(ST_Table* st, string var_name) {
    for (int i = 0; i < ST_Length; i++) {
        if (st[i]->Var_name == var_name)
            return i;
    }
    return -1;
}

/*
* 在ST表中寻找函数
*/
int Fun_Find(ST_Table* ST, string fun_name, string type) {
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == fun_name && ST[i]->Var_type == type)
            return i;
    }
    return -1;
}

/*
变量声明
*/
void Declare_Variable(string type, ST_Table* ST, Memory* Mem) { //声明变量
    string var_name; //声明的变量名
    cin >> var_name;
    int end_with_fh = str_match(var_name, ';'); //判断是否以分号结尾
   if (end_with_fh == -1){
        cout << "Input Invalid!\n";
        return;
   }
   else {
       var_name = var_name.substr(0, var_name.length() - 1);
   }
   
    //截取变量名
    int var_len = var_name.length();
    string var;
    int index_var = 0;
    int flag = 0;
    var = var_name;

     //已经获取到变量名，判断是否重复声明
    Judge_Var_Repeat(ST, Mem, type, var);
    var = "";
    index_var = 0;
    flag = 1;
 
}

/*
判断变量是否重复声明
*/
void Judge_Var_Repeat(ST_Table* ST, Memory* Mem, string var_type, string var) {
    if (Variable_Find(ST, var) != -1) {
        cout << var << " has existed!\n";
        return;
    }
    else {
        Declare_Write(ST, Mem, var_type, var);
    }
}

/*
* 判断函数是否重复
*/
void Judge_Fun_Repeat(ST_Table* ST, Memory* Mem, string var_type, string var) {
    if (Fun_Find(ST, var, var_type) != -1) {
        cout << var << " has existed\n";
        return;
    }
    else {
        Declare_Write(ST, Mem, var_type, var);
    }
}


/*
声明变量时将信息写入ST表
*/
void Declare_Write(ST_Table* ST, Memory* Mem, string var_type, string var) {
    int addr = Calculate_Address(Mem);
    Write_ST(ST, var, addr, var_type);
}

/*计算地址
    return -1表示内存已经耗尽
*/
int Calculate_Address(Memory* Mem) {
    for (int i = 0; i < Mem_Length; i++) {
        if (Mem[i]->flag == 0)//表示这块内存还没被用过
        {
            Mem[i]->flag = 1;
            return i;
        }
    }
    return -1;
}


/*
真正地把变量信息写入符号表中
*/
void Write_ST(ST_Table* ST, string var_name, int address, string type) {
    for (int i = 0; i < ST_Length; i++)
    {
        if (ST[i]->Var_name.empty()) {
            ST[i]->Var_name = var_name;
            ST[i]->address = address;
            ST[i]->Var_type = type;
            break;
        }
    }
}

/*
对变量赋值进行处理
*/
void Variable_Assignment(ST_Table* ST, Memory* Mem, string cmd) {
    int pos = str_match(cmd, '=');//找到等号的位置,不要有空格
    string var = cmd.substr(0, pos);
    string content = cmd.substr(pos + 1, cmd.length()-(pos+1)-1); 
    //判断变量是否被声明
    if (Variable_Find(ST, var) == -1) {
        cout << "Please declare " << var << " first!\n" << endl;
        return;
    }
    int f = 0;

    if (Variable_Find(ST, content) == -1) { //针对b=a+1;这种右边的等式不是已有变量的情况
        for (int i = 0; i < ST_Length; i++) {
            if (ST[i]->Var_name == var) {
                int addr = ST[i]->address;

                for (int j = 0; j < Mem_Length; j++) {
                    if (j == addr) {
                        Mem[j]->content = content;
                        f = 1;
                        break;
                    }
                }
            }
            if (f == 1)
                break;
        }
    }
    else { //针对b=a这种情况
        string value;
        for (int i = 0; i < ST_Length; i++) {
            if (ST[i]->Var_name == content) {
                int addr = ST[i]->address;
                for (int j = 0; j < Mem_Length; j++) {
                    if (j == addr) {
                        value = Mem[j]->content;
                        f = 1;
                        break;
                    }
                }
            }
            if (f == 1)
                break;
        }
        if (f == 0) {
            cout << content << " not define!\n";
        }
        else {
            f = 0;
            for (int i = 0; i < ST_Length; i++) {
                if (ST[i]->Var_name == var) {
                    int addr = ST[i]->address;

                    for (int j = 0; j < Mem_Length; j++) {
                        if (j == addr) {
                            Mem[j]->content = value;
                            f = 1;
                            break;
                        }
                    }
                }
                if (f == 1)
                    break;
            }
        }
    }


   
}


/*
* 对函数进行赋值
*/
void Fun_Assignment(ST_Table* ST, Memory* Mem, string fun_name, string fun_type, string fun_body) {
    int f = 0;
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == fun_name && ST[i]->Var_type == fun_type) {
            int addr = ST[i]->address;
            for (int j = 0; j < Mem_Length; j++) {
                if (j == addr) {
                    Mem[j]->content = fun_body;
                    f = 1;
                    break;
                }
            }
        }
        if (f == 1)
            break;
    }
}
/*
* 打印符号表
*/
void Print_ST(ST_Table* ST)
{
    cout << "————————————————————————————————————————\n";
    cout << "var\tAddress\tType\n";
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name != "") {
            cout << ST[i]->Var_name << "\t" << ST[i]->address << "\t" << ST[i]->Var_type << endl;
        }
    }
    cout << "————————————————————————————————————————\n";
}

/*
* 打印内存表
*/
void Print_Mem(Memory* M) {
    cout << "————————————————————————————————————————\n";
    cout << "Address\tContent\n";
    for (int i = 0; i < Mem_Length; i++) {
        if (M[i]->flag == 1)
            cout << i << "\t" << M[i]->content << endl;
    }
    cout << "————————————————————————————————————————\n";
}


/*
* 获得变量的地址
*/
void Get_Address(ST_Table* ST, string cmd) {
    int pos = str_match(cmd, '&');
    int pos2 = str_match(cmd, ';');
    if (pos2 == -1) {
        cout << "Input Invalid!\n";
        return;
    }
    string var = cmd.substr(pos + 1, pos2 - pos - 1);
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == var) {
            cout << ST[i]->address << endl;
            break;
        }
    }
}

/*
* 获得变量的值:$a;
*/
string Get_Value(ST_Table* ST, Memory* Mem, string cmd)
{
    int pos = str_match(cmd, '$');
    int pos2 = str_match(cmd, ';');
    if (pos2 == -1) {
        cout << "Input Invalid!\n";
        return "";
    }
    string var = cmd.substr(pos + 1, pos2 - pos - 1);

    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == var) {
            int addr = ST[i]->address;
            for (int j = 0; j < Mem_Length; j++) {
                if (j == addr) {
                    return Mem[j]->content;
 
                }
            }
        }

    }
}

/*
* 判断是否为int型
*/
int is_Int(string var) {
    int flag = 0;
    for (int i = 0; i < var.length(); i++) {
        if (isdigit(var[i]))
            flag = 1;
        else
            flag = 0;
    }
    return flag;
}

/*
判断左元和右元是否为同一类型
*/
int is_Same_Type(ST_Table* ST, string left, string right) {
    int int_left = 0;
    int int_right = 0;
    if (is_Int(left) == 1) {//左元为数字
        int_left = stoi(left);
        if (is_Int(right) == 1) { //右元为数字
            return 1;
        }
        else {//右元为string,判断string存的值是否为int
            string right_type = Get_Type(ST, right);
            if (right_type == "int") { //左边为数字，右边为(string)数字
                return 2;
            }
            else //类型不兼容
            {
                return 0;//左数右string
            }
        }
    }
    else { //左元为string
        string left_type = Get_Type(ST, left);
        if (left_type == "string") { //左元所存的值为string
            string right_type = Get_Type(ST, right);//右元的type,可能为string，也可能为int
            if (right_type == left_type) { //右元所存的值也为string
                return 4; //左string右string
            }
            else if(right_type=="")
                return 5;
           
        }
        else {//左元所存为int
            if (is_Int(right) == 1) { //左(string)数字右数字
                return 3;
            }
            else { //右元表面为string
                string right_type = Get_Type(ST, right);//右元的type
                if (right_type == "int") { //右元所存的值为int
                    return 2;
                }
                else { //左int右string
                    return 0;
                }
            }
        }

    }
}

/*
* 返回var_name的值
*/
string Fetch_Value(ST_Table* ST, Memory* Mem, string var_name) {
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == var_name) {
            int addr = ST[i]->address;
                for (int j = 0; j < Mem_Length; j++) {
                    if (j == addr) {
                        return Mem[j]->content;
                    }
                }
        }
    }
}

/*修改Mem中int类型的值
*/
void Modify_Mem_int(ST_Table* ST, Memory* Mem, string var_name,int new_value) {
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == var_name) {
            int addr = ST[i]->address;
            for (int j = 0; j < Mem_Length; j++) {
                if (j == addr) {
                    Mem[j]->content = to_string(new_value);
                }
            }
        }
    }
}
/*
修改Mem中string类型的值
*/
void Modify_Mem_string(ST_Table* ST, Memory* Mem, string var_name, string new_value) {
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == var_name) {
            int addr = ST[i]->address;
            for (int j = 0; j < Mem_Length; j++) {
                if (j == addr) {
                    Mem[j]->content = new_value;
                }
            }
        }
    }
}

/*
* 加法计算，区分Int和String
*/
void Variable_Add(ST_Table* ST, Memory* Mem, string cmd)
{
    int pos = str_match(cmd, '='); //=的位置
    int pos_end = str_match(cmd, ';');//;的位置
    string left_num;//左元
    string right_num;//右元
    if (pos != -1) {
        string var = cmd.substr(0, pos);//要存结果的变量
        string type1 = Get_Type(ST, var);//获取var的类型
        string dh_right = cmd.substr(pos + 1, pos_end - pos);
        int pos_add;//加号的位置
        pos_add = str_match(dh_right, '+');
        int right_end;//新的子串的分号的位置
        right_end = str_match(dh_right, ';');
        string left = dh_right.substr(0, pos_add);//左元
        string right = dh_right.substr(pos_add+1, right_end-pos_add-1);//右元
        int is_same_type = is_Same_Type(ST, left, right);
        if (is_same_type==0) {
            cout << "Two Type not same\n";
            return;
        }
        else {
            if (is_same_type == 1) //int int
            {
                if (type1 == "int") { 
                    int left_value = atoi(left.c_str());
                    int right_value = atoi(right.c_str());
                    int ans = left_value + right_value;
                    Modify_Mem_int(ST, Mem, var, ans);

                }
                else {
                    cout << "Three Type not same\n";
                    return;
                }
            }
            else if (is_same_type == 2) {//int,string(int)
                if (type1 == "int") {
                    int left_value = atoi(left.c_str());
                    string right_content = Fetch_Value(ST, Mem, right);
                    int right_value = atoi(right_content.c_str());
                    int ans = left_value + right_value;
                  /*  string content = Fetch_Value(ST, Mem, var);
                    int con = atoi(content.c_str());
                    ans = ans + con;*/
                    Modify_Mem_int(ST, Mem, var, ans);
                }
                else {
                    cout << "Three Type not same\n";
                    return;
                }
            }
            else if (is_same_type == 3) { //string(int),int
                if (type1 == "int") {
                   
                    string left_content = Fetch_Value(ST, Mem, left);
                    int left_value = atoi(left_content.c_str());
                    int right_value = atoi(right.c_str());
                    int ans = left_value + right_value;
                  /*  string content = Fetch_Value(ST, Mem, var);
                    int con = atoi(content.c_str());
                    ans = ans + con;*/
                    Modify_Mem_int(ST, Mem, var, ans);
                }
                else {
                    cout << "Three Type not same\n";
                    return;
                }
            }
            else if (is_same_type == 4) { //string,string
                if (type1 == "string") {
                    string left_value = Fetch_Value(ST, Mem, left);
                    string right_value = Fetch_Value(ST, Mem, right);
                    string ans = left_value + right_value;
                    Modify_Mem_string(ST, Mem, var, ans);
                }
                else {
                    cout << "Three Type not same\n";
                    return;
                }
            }
            else if (is_same_type == 5) { //string+int
                if (type1 == "string") {
                    string left_value = Fetch_Value(ST, Mem, left);
                    string right_value = right;
                    string ans = left_value + right_value;
                    Modify_Mem_string(ST, Mem, var, ans);
                }
                else {
                    cout << "Three Type not same\n";
                    return;
                }
            }
            else if (is_same_type == 6) {//int +string
                if (type1 == "string") {
                    string left_value = left;
                    string right_value = Fetch_Value(ST, Mem, right);
                    string ans = left_value + right_value;
                    Modify_Mem_string(ST, Mem, var, ans);
                }
                else {
                    cout << "Three Type not same\n";
                    return;
                }

            }
        }
       

    }
    else {//显示临时变量temp
        int pos_add = str_match(cmd, '+');
        left_num = cmd.substr(0, pos_add);//左元
        string left = left_num;
        right_num = cmd.substr(pos_add + 1, pos_end - pos_add - 1);//右元
        string right = right_num;
        int is_same_type = is_Same_Type(ST, left, right);
        if (is_same_type == 0) {
            cout << "Two Type not same\n";
            return;
        }
        else {
            if (is_same_type == 1) //int int
            {
                    int left_value = atoi(left.c_str());
                    int right_value = atoi(right.c_str());
                    int ans = left_value + right_value;
                    temp = to_string(ans);
                    cout << temp << endl;
                    
            }
            else if (is_same_type == 2) {//int,string(int)
               
                    int left_value = atoi(left.c_str());
                    string right_content = Fetch_Value(ST, Mem, right);
                    int right_value = atoi(right_content.c_str());
                    int ans = left_value + right_value;
                    temp = to_string(ans);
                    cout << temp << endl;
             
               
            }
            else if (is_same_type == 3) { //string(int),int
               
                    string left_content = Fetch_Value(ST, Mem, left);
                    int left_value = atoi(left_content.c_str());
                    int right_value = atoi(right.c_str());
                    int ans = left_value + right_value;
                    temp = to_string(ans);
                    cout << temp << endl;
            }
            else if (is_same_type == 4) { //string,string
              
                    string left_value = Fetch_Value(ST, Mem, left);
                    string right_value = Fetch_Value(ST, Mem, right);
                    string ans = left_value + right_value;
                    temp = left_value + right_value;
                    cout << temp << endl;
            }
        }
    }



}
/*
* 获得var的类型
*/
string Get_Type(ST_Table* ST, string var)
{
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == var) {
            return ST[i]->Var_type;
        }
    }
    return "";
}
/*
* 获得string的长度
*/
int Get_String_Len(ST_Table* ST, Memory* Mem, string cmd)
{   
    int pos_left = str_match(cmd, '(');
    int pos_right = str_match(cmd, ')');
    int pos_end = str_match(cmd, ';');
    string var = cmd.substr(pos_left + 1, pos_right - pos_left-1);
    int len = 0;
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == var)
        {
            int addr = ST[i]->address;
            for (int j = 0; j < Mem_Length; j++) {
                if (j == addr) {
                    len=Mem[j]->content.length();
                    break;
                }
            }
        }
        if (len != 0)
            break;
    }
    return len;
}

/*
* 减法操作
* var:等号左边的变量
* content:等号右边的内容，除去分号
* op:操作符
*/
void Del_Operate(ST_Table* ST, Memory* Mem, string var,string content,char op) {
    int pos_op;//操作符的位置
    pos_op = str_match(content, op);
    int right_end;//新的子串的分号的位置
    right_end = str_match(content, ';');
    string left = content.substr(0, pos_op);//左元
    string right = content.substr(pos_op + 1, right_end - pos_op - 1);//右元
    int is_same_type = is_Same_Type(ST, left, right);
    string type1 = Get_Type(ST, var);
    if (is_same_type == 0) {
        cout << "Two Type not same\n";
        return;
    }
    else {
        if (is_same_type == 1) //int int
        {
            if (type1 == "int") {
                int left_value = atoi(left.c_str());
                int right_value = atoi(right.c_str());
                int ans = left_value - right_value;
                Modify_Mem_int(ST, Mem, var, ans);

            }
            else {
                cout << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 2) {//int,string(int)
            if (type1 == "int") {
                int left_value = atoi(left.c_str());
                string right_content = Fetch_Value(ST, Mem, right);
                int right_value = atoi(right_content.c_str());
                int ans = left_value - right_value;

                Modify_Mem_int(ST, Mem, var, ans);
            }
            else {
                cout << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 3) { //string(int),int
            if (type1 == "int") {

                string left_content = Fetch_Value(ST, Mem, left);
                int left_value = atoi(left_content.c_str());
                int right_value = atoi(right.c_str());
                int ans = left_value - right_value;
                Modify_Mem_int(ST, Mem, var, ans);
            }
            else {
                cout << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 4) { //string,string
            cout << "string don't support " << op << endl;
            return;
        }
    }
}

/*
* 乘法
*/
void Mul_Operate(ST_Table* ST, Memory* Mem, string var, string content, char op) {
    int pos_op;//操作符的位置
    pos_op = str_match(content, op);
    int right_end;//新的子串的分号的位置
    right_end = str_match(content, ';');
    string left = content.substr(0, pos_op);//左元
    string right = content.substr(pos_op + 1, right_end - pos_op - 1);//右元
    int is_same_type = is_Same_Type(ST, left, right);
    string type1 = Get_Type(ST, var);
    if (is_same_type == 0) {
        cout << "Two Type not same\n";
        return;
    }
    else {
        if (is_same_type == 1) //int int
        {
            if (type1 == "int") {
                int left_value = atoi(left.c_str());
                int right_value = atoi(right.c_str());
                int ans = left_value * right_value;
                Modify_Mem_int(ST, Mem, var, ans);

            }
            else {
                cout << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 2) {//int,string(int)
            if (type1 == "int") {
                int left_value = atoi(left.c_str());
                string right_content = Fetch_Value(ST, Mem, right);
                int right_value = atoi(right_content.c_str());
                int ans = left_value * right_value;

                Modify_Mem_int(ST, Mem, var, ans);
            }
            else {
                cout << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 3) { //string(int),int
            if (type1 == "int") {

                string left_content = Fetch_Value(ST, Mem, left);
                int left_value = atoi(left_content.c_str());
                int right_value = atoi(right.c_str());
                int ans = left_value * right_value;
                Modify_Mem_int(ST, Mem, var, ans);
            }
            else {
                cout << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 4) { //string,string
            cout << "string don't support " << op << endl;
            return;
        }
    }
}

/*
* 除法
*/
void Div_Operate(ST_Table* ST, Memory* Mem, string var, string content, char op) {
    int pos_op;//操作符的位置
    pos_op = str_match(content, op);
    int right_end;//新的子串的分号的位置
    right_end = str_match(content, ';');
    string left = content.substr(0, pos_op);//左元
    string right = content.substr(pos_op + 1, right_end - pos_op - 1);//右元
    int is_same_type = is_Same_Type(ST, left, right);
    string type1 = Get_Type(ST, var);
    if (is_same_type == 0) {
        cout << "Two Type not same\n";
        return;
    }
    else {
        if (is_same_type == 1) //int int
        {
            if (type1 == "int") {
                int left_value = atoi(left.c_str());
                int right_value = atoi(right.c_str());
                int ans = left_value / right_value;
                Modify_Mem_int(ST, Mem, var, ans);

            }
            else {
                cout << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 2) {//int,string(int)
            if (type1 == "int") {
                int left_value = atoi(left.c_str());
                string right_content = Fetch_Value(ST, Mem, right);
                int right_value = atoi(right_content.c_str());
                int ans = left_value / right_value;

                Modify_Mem_int(ST, Mem, var, ans);
            }
            else {
                cout << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 3) { //string(int),int
            if (type1 == "int") {

                string left_content = Fetch_Value(ST, Mem, left);
                int left_value = atoi(left_content.c_str());
                int right_value = atoi(right.c_str());
                int ans = left_value / right_value;
                Modify_Mem_int(ST, Mem, var, ans);
            }
            else {
                cout << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 4) { //string,string
            cout << "string don't support " << op << endl;
            return;
        }
    }
}

/*
* 临时变量的减法
*/
void Temp_Del_Operate(ST_Table* ST, Memory* Mem, string content, char op) {
    int pos_op = str_match(content, op);
    string left_num = content.substr(0, pos_op);//左元
    string left = left_num;
    string right_num = content.substr(pos_op + 1, content.length()- pos_op -1);//右元
    string right = right_num;
    int is_same_type = is_Same_Type(ST, left, right);
    if (is_same_type == 0) {
        cout << "Two Type not same\n";
        return;
    }
    else {
        if (is_same_type == 1) //int int
        {
            int left_value = atoi(left.c_str());
            int right_value = atoi(right.c_str());
            int ans = left_value - right_value;
            temp = to_string(ans);
            cout << temp << endl;

        }
        else if (is_same_type == 2) {//int,string(int)

            int left_value = atoi(left.c_str());
            string right_content = Fetch_Value(ST, Mem, right);
            int right_value = atoi(right_content.c_str());
            int ans = left_value - right_value;
            temp = to_string(ans);
            cout << temp << endl;


        }
        else if (is_same_type == 3) { //string(int),int

            string left_content = Fetch_Value(ST, Mem, left);
            int left_value = atoi(left_content.c_str());
            int right_value = atoi(right.c_str());
            int ans = left_value - right_value;
            temp = to_string(ans);
            cout << temp << endl;
        }
        else if (is_same_type == 4) { //string,string

            cout << "string don't support " << op << endl;
        }
    }
}

/*
* 临时变量的乘法
*/
void Temp_Mul_Operate(ST_Table* ST, Memory* Mem, string content, char op) {
    int pos_op = str_match(content, op);
    string left_num = content.substr(0, pos_op);//左元
    string left = left_num;
    string right_num = content.substr(pos_op + 1, content.length() - pos_op - 1);//右元
    string right = right_num;
    int is_same_type = is_Same_Type(ST, left, right);
    if (is_same_type == 0) {
        cout << "Two Type not same\n";
        return;
    }
    else {
        if (is_same_type == 1) //int int
        {
            int left_value = atoi(left.c_str());
            int right_value = atoi(right.c_str());
            int ans = left_value * right_value;
            temp = to_string(ans);
            cout << temp << endl;

        }
        else if (is_same_type == 2) {//int,string(int)

            int left_value = atoi(left.c_str());
            string right_content = Fetch_Value(ST, Mem, right);
            int right_value = atoi(right_content.c_str());
            int ans = left_value * right_value;
            temp = to_string(ans);
            cout << temp << endl;


        }
        else if (is_same_type == 3) { //string(int),int

            string left_content = Fetch_Value(ST, Mem, left);
            int left_value = atoi(left_content.c_str());
            int right_value = atoi(right.c_str());
            int ans = left_value * right_value;
            temp = to_string(ans);
            cout << temp << endl;
        }
        else if (is_same_type == 4) { //string,string

            cout << "string don't support " << op << endl;
        }
    }
}

/*
* 临时变量的除法
*/
void Temp_Div_Operate(ST_Table* ST, Memory* Mem, string content, char op) {
    int pos_op = str_match(content, op);
    string left_num = content.substr(0, pos_op);//左元
    string left = left_num;
    string right_num = content.substr(pos_op + 1, content.length() - pos_op - 1);//右元
    string right = right_num;
    int is_same_type = is_Same_Type(ST, left, right);
    if (is_same_type == 0) {
        cout << "Two Type not same\n";
        return;
    }
    else {
        if (is_same_type == 1) //int int
        {
            int left_value = atoi(left.c_str());
            int right_value = atoi(right.c_str());
            int ans = left_value / right_value;
            temp = to_string(ans);
            cout << temp << endl;

        }
        else if (is_same_type == 2) {//int,string(int)

            int left_value = atoi(left.c_str());
            string right_content = Fetch_Value(ST, Mem, right);
            int right_value = atoi(right_content.c_str());
            int ans = left_value / right_value;
            temp = to_string(ans);
            cout << temp << endl;


        }
        else if (is_same_type == 3) { //string(int),int

            string left_content = Fetch_Value(ST, Mem, left);
            int left_value = atoi(left_content.c_str());
            int right_value = atoi(right.c_str());
            int ans = left_value / right_value;
            temp = to_string(ans);
            cout << temp << endl;
        }
        else if (is_same_type == 4) { //string,string

            cout << "string don't support " << op << endl;
        }
    }
}

/*
* Int的减法,乘法，除法操作
*/
void Int_OtherOperate(ST_Table* ST, Memory* Mem, string cmd)
{
    int pos = str_match(cmd, '=');
    int pos_end = str_match(cmd, ';');
    if (pos != -1) {
        //找操作符的位置
        string var = cmd.substr(0, pos);
        string content = cmd.substr(pos + 1, pos_end - pos - 1);
        int pos_del = str_match(cmd, '-');
        int pos_mul = str_match(cmd, '*');
        int pos_div = str_match(cmd, '/');
        if (pos_del != -1) {//减法
            Del_Operate(ST, Mem, var, content, '-');
        }
        else if (pos_mul != -1) {//乘法
            Mul_Operate(ST, Mem, var, content, '*');
        }
        else if (pos_div != -1) {//除法
            Div_Operate(ST, Mem, var, content, '/');
        }
        else {
            cout << "Input Operater Invalid!\n";
            return;
        }
    }
    else {//调用临时变量
        string content = cmd.substr(0, cmd.length() - 1);
        int pos_del = str_match(cmd, '-');
        int pos_mul = str_match(cmd, '*');
        int pos_div = str_match(cmd, '/');
        if (pos_del != -1) {//减法
            Temp_Del_Operate(ST, Mem, content, '-');
        }
        else if (pos_mul != -1) {//乘法
            Temp_Mul_Operate(ST, Mem, content, '*');
        }
        else if (pos_div != -1) {//除法
            Temp_Div_Operate(ST, Mem, content, '/');
        }
        else {
            cout << "Input Operater Invalid!\n";
            return;
        }
    }
}

/*
* 在String_Compare中使用
*/
void Compare_String(ST_Table* ST, Memory* Mem, string left_value, string right_value,char op) {
    string ans;
    int len = left_value.length() > right_value.length() ? left_value.length() : right_value.length();
    int left_len = left_value.length();
    int right_len = right_value.length();
    if (op == '|') {
        //开始比较
        for (int i = 0; i < len; i++) {
            char c1 = left_value[i];
            char c2 = right_value[i];
            if (c1 > c2) {
                ans += c1;
            }
            else {
                ans += c2;
            }
            left_len--;
            right_len--;
            if (left_len == 0 && right_len!=0) {
                string s= right_value.substr(right_value.length() - right_len, right_value.length() - left_value.length());
                ans += s;
                cout << ans << endl;
                return;
            }
            else if (right_len == 0 && left_len!=0) {
                string s= left_value.substr(left_value.length() - left_len, left_value.length() - right_value.length());
                ans += s;
                cout << ans << endl;
                return;
            }
            else if (left_len == 0 && right_len == 0) {
                cout << ans << endl;
                return;
            }

        }
    }
    else if(op=='^') {
        //开始比较
        for (int i = 0; i < len; i++) {
            char c1 = left_value[i];
            char c2 = right_value[i];
            if (c1 < c2) {
                ans += c1;
            }
            else {
                ans += c2;
            }
            left_len--;
            right_len--;
            if (left_len == 0 && right_len != 0) {
                string s = right_value.substr(right_value.length() - right_len, right_value.length() - left_value.length());
                ans += s;
                cout << ans << endl;
                return;
            }
            else if (right_len == 0 && left_len != 0) {
                string s = left_value.substr(left_value.length() - left_len, left_value.length() - right_value.length());
                ans += s;
                cout << ans << endl;
                return;
            }
            else if (left_len == 0 && right_len == 0) {
                cout << ans << endl;
                return;
            }

        }
    }
   
}


/*
* 去除string中的所有空格
*/
void trim(string& s) {
    int len = s.length();
    string ss;
    for (int i = 0; i < len; i++) {
        if (s[i] != ' ')
            ss += s[i];
    }
    s = ss;
}
/*
* string按位取大或取小
*/
void String_Compare(ST_Table* ST, Memory* Mem, string cmd)
{
    int pos_big = str_match(cmd, '|');
    int pos_small = str_match(cmd, '^');
    if (pos_big != -1) {//取大
        string left = cmd.substr(0, pos_big);//左元
        string right = cmd.substr(pos_big + 1, cmd.length() - pos_big - 2);//右元
        int l_f = Variable_Find(ST, left);
        int r_f = Variable_Find(ST, right);
        if (l_f != -1 && r_f != -1) {//string变量+string变量
            string left_value;
            string right_value;
            left_value = Fetch_Value(ST, Mem, left);
            right_value = Fetch_Value(ST, Mem, right);
            Compare_String(ST, Mem, left_value, right_value,'|');
            
        }
        else if (l_f != -1 && r_f == -1) {//变量+string常量
            string left_value = Fetch_Value(ST, Mem, left);
            string right_value = right;
            Compare_String(ST, Mem, left_value, right_value, '|');
        }
        else if (l_f == -1 && r_f != -1) {//string常量+变量
            string left_value = left;
            string right_value = Fetch_Value(ST, Mem, right);
            Compare_String(ST, Mem, left_value, right_value, '|');
        }
        else {
            cout << "Type Wrong\n";
            return;
        }
    }
    else if (pos_small != -1) {//取小
        string left = cmd.substr(0, pos_small);//左元
        string right = cmd.substr(pos_small + 1, cmd.length() - pos_small - 2);//右元
        int l_f = Variable_Find(ST, left);
        int r_f = Variable_Find(ST, right);
        if (l_f != -1 && r_f != -1) {//string变量+string变量
            string left_value;
            string right_value;
            left_value = Fetch_Value(ST, Mem, left);
            right_value = Fetch_Value(ST, Mem, right);
            Compare_String(ST, Mem, left_value, right_value, '^');

        }
        else if (l_f != -1 && r_f == -1) {//变量+string常量
            string left_value = Fetch_Value(ST, Mem, left);
            string right_value = right;
            Compare_String(ST, Mem, left_value, right_value, '^');
        }
        else if (l_f == -1 && r_f != -1) {//string常量+变量
            string left_value = left;
            string right_value = Fetch_Value(ST, Mem, right);
            Compare_String(ST, Mem, left_value, right_value, '^');
        }
        
        else {
            cout << "Type Wrong\n";
            return;
        }
    }
    else {
        cout << "Input Invalid!\n";
        return;
    }
}


/*
* 定义函数
*/
void Fun_Define(ST_Table* ST, Memory* Mem, string cmd)
{
    string fun_name;
    cin >> fun_name;
    fun_name = fun_name.substr(0, fun_name.length() - 1);
    getchar();

    cout << "-->";
    string return_define;
    getline(cin, return_define);
    trim(return_define);
    string return_type = return_define.substr(10, return_define.length() - 10-1);

    cout << "-->";
    string argu_define;
    getline(cin, argu_define);
    int pos_first_space = str_match(argu_define, ' ');
    int pos_end_argu = str_match(argu_define, ';');
    string argu_type = argu_define.substr(pos_first_space + 1, pos_end_argu - pos_first_space - 1);

    cout << "....";
    string fun_body;//完整函数体
    string statement;
    string fun_body_noback;//无换行的函数体

    getline(cin, statement);
    while (statement.substr(0, 6) != "return") {
        fun_body += statement + '\n';
        fun_body_noback += statement;
        cout << "....";
        getline(cin, statement);
    }
    fun_body += statement+'\n';
    fun_body_noback += statement;
    string end_define;
    cout << "-->";
    getline(cin, end_define);
    string type = "fun("+argu_type + "," + return_type+")";
    //把returntype和argutype，写入ST中，把fun_body放入Mem中
    Judge_Fun_Repeat(ST, Mem,type ,fun_name); //完成ST表的填写
    Fun_Assignment(ST, Mem, fun_name, type, fun_body_noback);


    cout << ">>>";
    cout << "Fun body:" << endl;
    cout << fun_body;
    return;
}



int main() {
    ST_Table* ST;
    Memory* Mem;
    ST = new ST_Table[ST_Length];
    Mem = new Memory[Mem_Length];
    Init_ST(ST);
    Init_Mem(Mem);
    cout << "Welocome to U Language!\n";
    while (1) {
        cout << ">>>";
        string cmd = "";
        cin >> cmd;
        if (cmd == "int" || cmd == "string") {
            Declare_Variable(cmd, ST, Mem);
        }
        else if (str_match(cmd, '=') != -1 && str_match(cmd, '+') == -1 && str_match(cmd, ';') != -1 && str_match(cmd, '-') == -1
            && str_match(cmd, '*') == -1 && str_match(cmd, '/') == -1) {
            Variable_Assignment(ST, Mem, cmd);
        }
        else if (cmd == "exit;") {
            break;
        }
        else if (cmd == "Print_ST;") {
            Print_ST(ST);
        }
        else if (cmd == "Print_Mem;") {
            Print_Mem(Mem);
        }
        else if (str_match(cmd, '&') != -1) {
            Get_Address(ST, cmd);
        }
        else if (str_match(cmd, '$') != -1) {
            cout << Get_Value(ST, Mem, cmd) << endl;
        }
        else if (str_match(cmd, '+') != -1) {
            Variable_Add(ST, Mem, cmd);
        }
        else if (cmd.substr(0, 4) == "len(") {
            int len = Get_String_Len(ST, Mem, cmd);
            if (len != 0) {
                cout << len << endl;
            }
            else {
                cout << "Not Define or Assigment" << endl;
            }
        }
        else if (str_match(cmd, '|')!=-1 || str_match(cmd, '^')!=-1) {
            String_Compare(ST, Mem, cmd);
        }
        else if (str_match(cmd, '-')!=-1 || str_match(cmd, '*')!=-1 || str_match(cmd, '/')!=-1) {
            Int_OtherOperate(ST, Mem, cmd);
        }
        else if (cmd.substr(0, 6) == "define") {
            Fun_Define(ST, Mem, cmd);
            
        }
        else if (cmd=="\n") {
     
        }
        else
            cout << "Please input right command!\n";
    }
    delete(ST);
    delete(Mem);
    return 0;
}