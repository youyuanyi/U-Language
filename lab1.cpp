#include<iostream>
#include<cstring>
#include<string>
#include<cstdlib>
#include<math.h>
#include<vector>
#define ST_Length 100 //���ű���
#define Mem_Length 100 //�ڴ����
using namespace std;

string temp;//��ʱ����
//��Ϊ�������汾���⣬����ʱҪ����-fpermissive

//���ű�
typedef struct {
    string Var_name; //������
    int     address;    //������ַ
    string Var_type;   //��������
}ST_Table[ST_Length];

//�ڴ��
typedef struct {
    int flag;   //�Ƿ�ʹ��
    string content; //����
}Memory[Mem_Length];

void Init_ST(ST_Table* ST);//��ʼ�����ű�
void Init_Mem(Memory* M);//��ʼ���ڴ��
int str_match(string str, char c);    //�ַ���ƥ�䣬����Ҫ�ҵ��ַ����򷵻��ַ�λ��i�����򷵻�-1
int Variable_Find(ST_Table* ST, string var_name);//���ҷ��ű��еı������������򷵻ط��ű��±꣬�������򷵻�-1
void Declare_Variable(string type, ST_Table* ST, Memory* Mem);  //���������ͱ���
void Judge_Var_Repeat(ST_Table* ST, Memory* Mem, string var_type, string var); //�Ա������ظ�����ʱ���ж�
void Declare_Write(ST_Table* ST, Memory* MEM, string var_type, string var); //��������ʱ����Ϣд��ST��
void Write_ST(ST_Table* ST, string var_name, int address, string type); //�������ı���������Ϣд����ű�ST
int Calculate_Address(Memory* Mem);    //���ڴ��ַ��ֵʱ�������������ʱӦ��д��ģ���ڴ���׵�ַ
void Variable_Assignment(ST_Table* ST, Memory* Mem, string  cmd_name);//��ST�������ı�����ֵ��д���ڴ�
void Print_ST(ST_Table* ST);//��ӡST
void Print_Mem(Memory* Mem); //��ӡMem
void Get_Address(ST_Table* ST, string cmd);//ȡ�ñ����ĵ�ַ
string Get_Value(ST_Table* ST, Memory* Mem, string cmd);//ȡ�ñ�����ֵ
void Variable_Add(ST_Table* ST, Memory* Mem, string cmd);//int��string�ļӷ�����
string Get_Type(ST_Table* ST, string var);//���var������
int Get_String_Len(ST_Table* ST, Memory* Mem, string var); //��ȡstring�ĳ���
void Int_OtherOperate(ST_Table* ST, Memory* Mem, string cmd);//�����ļ������˷�����������
void String_Compare(ST_Table* ST, Memory* Mem, string cmd);
void Fun_Define(ST_Table* ST, Memory* Mem, string cmd);//�������庯��
void If_Operate(ST_Table* ST, Memory* Mem, string cmd); //����if���
void Main_Switch(ST_Table* ST, Memory* Mem, string cmd);//ָ����ж�
void Fun_Call(ST_Table* ST, Memory* Mem, string cmd);//��������
string Return_Operate(ST_Table *ST, Memory* Mem, string cmd);
void Remove_var(ST_Table* ST, Memory* Mem,  int temp_num);//�����ʱ����
void Help();//��������

void Help() {
    cout << "֧��int,string,fun��������" << endl;
    cout << "int a;\t����һ��int�ͱ���a" << endl;
    cout << "string s:\t����һ��string�б���s" << endl;
    cout << "֧�ֱ�������������ֵ��ȡֵ��ȡ��ַ������Ȳ���" << endl;
    cout << "int��֧��+,-,*,/" << endl;
    cout << "string��֧��+,len(s)ȡs�ĳ���,|��λȡ��,^��λȡС" << endl;
    cout << "֧��if��䣬Ŀǰֻ֧��a>b������" << endl;
    cout << "֧����ʱ����temp,��������" << endl;
    cout << "Print_ST��ӡ���ű�\tPrint_Mem��ӡ�ڴ��" << endl;
    cout << "֧�ֶ�Ԫ�����Ķ���\tdefine Funname" << endl;
    cout << "֧�ֺ�������:call ����(����1,����2)" << endl;
    cout << "֧�־ֲ�����" << endl;

}
/*
��ʼ��ST��
*/
void Init_ST(ST_Table* ST)
{
    for (int i = 0; i < ST_Length; i++) {
        ST[i]->address = 0;
    }
}

/*
��ʼ��Mem*/
void Init_Mem(Memory* M)
{
    for (int i = 0; i < Mem_Length; i++) {
        M[i]->flag = 0;
    }
}

/*
�ַ�ƥ��
*/
int str_match(string str, char c) {
    for (int i = 0; i < str.length(); ++i)
        if (str[i] == c)
            return i;
    return -1;
}


/*
��st����Ѱ�ұ���
*/
int Variable_Find(ST_Table* st, string var_name) {
    for (int i = 0; i < ST_Length; i++) {
        if (st[i]->Var_name == var_name)
            return i;
    }
    return -1;
}

/*
* ��ST����Ѱ�Һ���
*/
int Fun_Find(ST_Table* ST, string fun_name, string type) {
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == fun_name && ST[i]->Var_type == type)
            return i;
    }
    return -1;
}



/*
��������
*/
void Declare_Variable(string cmd, ST_Table* ST, Memory* Mem) { //��������
    string var_name; //�����ı�����
    int pos_end = str_match(cmd, ';');
    int pos_space = str_match(cmd, ' ');
    var_name = cmd.substr(pos_space + 1, pos_end - pos_space - 1);
    string type = cmd.substr(0, pos_space);
   if (pos_end == -1){
        cout << "SyntaxError: Input Invalid!\n";
        return;
   }
  
   
    //��ȡ������
    int var_len = var_name.length();
    string var;
    int index_var = 0;
    int flag = 0;
    var = var_name;

     //�Ѿ���ȡ�����������ж��Ƿ��ظ�����
    Judge_Var_Repeat(ST, Mem, type, var);
    var = "";
    index_var = 0;
    flag = 1;
 
}

/*
�жϱ����Ƿ��ظ�����
*/
void Judge_Var_Repeat(ST_Table* ST, Memory* Mem, string var_type, string var) {
    if (Variable_Find(ST, var) != -1) {
        cout <<"Error: "<< var << " has existed!\n";
        return;
    }
    else {
        Declare_Write(ST, Mem, var_type, var);
    }
}

/*
* �жϺ����Ƿ��ظ�
*/
void Judge_Fun_Repeat(ST_Table* ST, Memory* Mem, string var_type, string var) {
    if (Fun_Find(ST, var, var_type) != -1) {
        cout <<"Error: " << var << " has existed\n";
        return;
    }
    else {
        Declare_Write(ST, Mem, var_type, var);
    }
}


/*
��������ʱ����Ϣд��ST��
*/
void Declare_Write(ST_Table* ST, Memory* Mem, string var_type, string var) {
    int addr = Calculate_Address(Mem);
    Write_ST(ST, var, addr, var_type);
}

/*�����ַ
    return -1��ʾ�ڴ��Ѿ��ľ�
*/
int Calculate_Address(Memory* Mem) {
    for (int i = 0; i < Mem_Length; i++) {
        if (Mem[i]->flag == 0)//��ʾ����ڴ滹û���ù�
        {
            Mem[i]->flag = 1;
            return i;
        }
    }
    return -1;
}


/*
�����ذѱ�����Ϣд����ű���
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
�Ա�����ֵ���д���
*/
void Variable_Assignment(ST_Table* ST, Memory* Mem, string cmd) {
    int pos = str_match(cmd, '=');//�ҵ��Ⱥŵ�λ��,��Ҫ�пո�
    string var = cmd.substr(0, pos);
    string content = cmd.substr(pos + 1, cmd.length()-(pos+1)-1); 
    //�жϱ����Ƿ�����
    if (Variable_Find(ST, var) == -1) {
        cout <<"ValueError: " << "Please declare " << var << " first!\n" << endl;
        return;
    }
    int f = 0;

    if (Variable_Find(ST, content) == -1) { //���b=a+1;�����ұߵĵ�ʽ�������б��������
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
    else { //���b=a�������
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
* �Ժ������и�ֵ
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
* ��ӡ���ű�
*/
void Print_ST(ST_Table* ST)
{
    cout << "��������������������������������������������������������������������������������\n";
    cout << "var\tAddress\tType\n";
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name != "") {
            cout << ST[i]->Var_name << "\t" << ST[i]->address << "\t" << ST[i]->Var_type << endl;
        }
    }
    cout << "��������������������������������������������������������������������������������\n";
}

/*
* ��ӡ�ڴ��
*/
void Print_Mem(Memory* M) {
    cout << "��������������������������������������������������������������������������������\n";
    cout << "Address\tContent\n";
    for (int i = 0; i < Mem_Length; i++) {
        if (M[i]->flag == 1)
            cout << i << "\t" << M[i]->content << endl;
    }
    cout << "��������������������������������������������������������������������������������\n";
}


/*
* ��ñ����ĵ�ַ
*/
void Get_Address(ST_Table* ST, string cmd) {
    int pos = str_match(cmd, '&');
    int pos2 = str_match(cmd, ';');
    if (pos2 == -1) {
        cout << "SyntaxError: Input Invalid!\n";
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
* ��ñ�����ֵ:$a;
*/
string Get_Value(ST_Table* ST, Memory* Mem, string cmd)
{
    int pos = str_match(cmd, '$');
    int pos2 = str_match(cmd, ';');
    if (pos2 == -1) {
        cout << "SyntaxError: Input Invalid!\n";
        return "";
    }
    string var = cmd.substr(pos + 1, pos2 - pos - 1);

    int f = Variable_Find(ST, var);
    if (f == -1) {
        cout <<"ValueError: " << var << " not existed!\n";
        return "";
    }
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
    return "";
}

/*
* �ж��Ƿ�Ϊint��
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
�ж���Ԫ����Ԫ�Ƿ�Ϊͬһ����
return:
0:���Ͳ�����
1:1+1;
2:1+a; a��ֵΪint
3:a+1; a��ֵΪint
4:string+string
5:a+b; a,b��ֵ����int
*/
int is_Same_Type(ST_Table* ST, string left, string right) {
    int int_left = 0;
    int int_right = 0;
    if (is_Int(left) == 1) {//��ԪΪ����
        int_left = stoi(left);
        if (is_Int(right) == 1) { //��ԪΪ����
            return 1;
        }
        else {//��ԪΪstring,�ж�string���ֵ�Ƿ�Ϊint
            string right_type = Get_Type(ST, right);
            if (right_type == "int") { //���Ϊ���֣��ұ�Ϊ(string)����
                return 2;
            }
            else {
                return 0;
            }
           
        }
    }
    else { //��ԪΪstring
        string left_type = Get_Type(ST, left);
        if (left_type == "string") { //��Ԫ�����ֵΪstring
            string right_type = Get_Type(ST, right);//��Ԫ��type,����Ϊstring��Ҳ����Ϊint
            if (right_type == left_type) { //��Ԫ�����ֵҲΪstring
                return 4; //��string��string
            }
            else
                return 0; //string+int�Ƿ�
           
        }
        else {//��ԪΪstring(int)
            if (is_Int(right) == 1) { //��(string)����������
                return 3;
            }
            else { //��Ԫ����Ϊstring
                string right_type = Get_Type(ST, right);//��Ԫ��type
                if (right_type == "int") { //��Ԫ�����ֵΪint
                    return 5;
                }
                else
                    return 0;
            }
        }

    }
}

/*
* ����var_name��ֵ
*/
string Fetch_Value(ST_Table* ST, Memory* Mem, string var_name) {
    string ans="";
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == var_name) {
            int addr = ST[i]->address;
                for (int j = 0; j < Mem_Length; j++) {
                    if (j == addr) {
                        ans = Mem[j]->content;
                        return ans;
                    }
                }
        }
    }
    return ans;
    
}

/*�޸�Mem��int���͵�ֵ
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
�޸�Mem��string���͵�ֵ
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
* �ӷ����㣬����Int��String
* �������Ϊ���֣��ұ�Ϊstringʱ����ӳ���
* return:
0:���Ͳ�����
1:1+1;
2:1+a; a��ֵΪint
3:a+1; a��ֵΪint
4:string+string
5:a+b; a,b��ֵ����int
*/
void Variable_Add(ST_Table* ST, Memory* Mem, string cmd)
{
    int pos = str_match(cmd, '='); //=��λ��
    int pos_end = str_match(cmd, ';');//;��λ��
    string left_num;//��Ԫ
    string right_num;//��Ԫ
    if (pos != -1) {
        string var = cmd.substr(0, pos);//Ҫ�����ı���
        string type1 = Get_Type(ST, var);//��ȡvar������
        string dh_right = cmd.substr(pos + 1, pos_end - pos);
        int pos_add;//�Ӻŵ�λ��
        pos_add = str_match(dh_right, '+');
        int right_end;//�µ��Ӵ��ķֺŵ�λ��
        right_end = str_match(dh_right, ';');
        string left = dh_right.substr(0, pos_add);//��Ԫ
        string right = dh_right.substr(pos_add+1, right_end-pos_add-1);//��Ԫ
        int is_same_type = is_Same_Type(ST, left, right);
        if (is_same_type==0) {
            cout <<"TypeError: " << "Two Type not same\n";
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
                    cout << "TypeError: "<<"Three Type not same\n";
                    return;
                }
            }
            else if (is_same_type == 2) {//int,string(int)
                if (type1 == "int") {
                    int left_value = atoi(left.c_str());
                    string right_content = Fetch_Value(ST, Mem, right);
                    int right_value = atoi(right_content.c_str());
                    int ans = left_value + right_value;
                    Modify_Mem_int(ST, Mem, var, ans);
                }
                else {
                    cout <<"TypeError: "<< "Three Type not same\n";
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
                    cout <<"TypeError: " << "Three Type not same\n";
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
                    cout <<"TypeError: " << "Three Type not same\n";
                    return;
                }
            }      
            else if (is_same_type == 5) {//string(int),string(int)
                if (type1 == "int") {
                    string left_value = Fetch_Value(ST, Mem, left);
                    string right_value = Fetch_Value(ST, Mem, right);
                    int left_int = atoi(left_value.c_str());
                    int right_int = atoi(right_value.c_str());
                    int ans = left_int + right_int;
                    Modify_Mem_int(ST, Mem, var, ans);
                }
            }
        }
       

    }
    else {//��ʾ��ʱ����temp
        int pos_add = str_match(cmd, '+');
        left_num = cmd.substr(0, pos_add);//��Ԫ
        string left = left_num;
        right_num = cmd.substr(pos_add + 1, pos_end - pos_add - 1);//��Ԫ
        string right = right_num;
        int is_same_type = is_Same_Type(ST, left, right);
        if (is_same_type == 0) {
            cout<<"TypeError: " << "Two Type not same\n";
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
            else if (is_same_type == 5) {//string(int),string(int)
                
                    string left_value = Fetch_Value(ST, Mem, left);
                    string right_value = Fetch_Value(ST, Mem, right);
                    int left_int = atoi(left_value.c_str());
                    int right_int = atoi(right_value.c_str());
                    temp = to_string(left_int + right_int);
                    cout << temp << endl;
                   
                }    
        }
    }



}
/*
* ���var������
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
* ���string�ĳ���
*/
int Get_String_Len(ST_Table* ST, Memory* Mem, string cmd)
{   
    int pos_left = str_match(cmd, '(');
    int pos_right = str_match(cmd, ')');
    int pos_end = str_match(cmd, ';');
    string var = cmd.substr(pos_left + 1, pos_right - pos_left-1);
    int f = Variable_Find(ST, var);
    if (f == -1)
    {
        cout <<"Error:" << var << " not exist!\n";
        return -1;
    }
    if (Get_Type(ST, var) != "string") {
        cout <<"TypeError:" << var << " is not string\n";
        return -1;
    }
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
* ��������
* var:�Ⱥ���ߵı���
* content:�Ⱥ��ұߵ����ݣ���ȥ�ֺ�
* op:������
*/
void Del_Operate(ST_Table* ST, Memory* Mem, string var,string content,char op) {
    int pos_op;//��������λ��
    pos_op = str_match(content, op);
    int right_end;//�µ��Ӵ��ķֺŵ�λ��
    right_end = str_match(content, ';');
    string left = content.substr(0, pos_op);//��Ԫ
    string right = content.substr(pos_op + 1, right_end - pos_op - 1);//��Ԫ
    int is_same_type = is_Same_Type(ST, left, right);
    string type1 = Get_Type(ST, var);
    if (is_same_type == 0) {
        cout <<"TypeError: " << "Two Type not same\n";
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
                cout << "TypeError: " << "Three Type not same\n";
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
                cout << "TypeError: " << "Three Type not same\n";
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
                cout << "TypeError: " << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 4) { //string,string
            cout << "TypeError: " << "string don't support " << op << endl;
            return;
        }
        else if (is_same_type == 5) { //string(int),string(int)
            string left_value = Fetch_Value(ST, Mem, left);
            string right_value = Fetch_Value(ST, Mem, right);
            int left_int = atoi(left_value.c_str());
            int right_int = atoi(right_value.c_str());
            int ans = left_int - right_int;
            Modify_Mem_int(ST, Mem, var, ans);
        }
    }
}

/*
* �˷�
*/
void Mul_Operate(ST_Table* ST, Memory* Mem, string var, string content, char op) {
    int pos_op;//��������λ��
    pos_op = str_match(content, op);
    int right_end;//�µ��Ӵ��ķֺŵ�λ��
    right_end = str_match(content, ';');
    string left = content.substr(0, pos_op);//��Ԫ
    string right = content.substr(pos_op + 1, right_end - pos_op - 1);//��Ԫ
    int is_same_type = is_Same_Type(ST, left, right);
    string type1 = Get_Type(ST, var);
    if (is_same_type == 0) {
        cout << "TypeError: " << "Two Type not same\n";
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
                cout << "TypeError: " << "Three Type not same\n";
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
                cout << "TypeError: " << "Three Type not same\n";
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
                cout << "TypeError: " << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 4) { //string,string
            cout << "TypeError: " << "string don't support " << op << endl;
            return;
        }
        else if (is_same_type == 5) { //string(int),string(int)
            string left_value = Fetch_Value(ST, Mem, left);
            string right_value = Fetch_Value(ST, Mem, right);
            int left_int = atoi(left_value.c_str());
            int right_int = atoi(right_value.c_str());
            int ans = left_int * right_int;
            Modify_Mem_int(ST, Mem, var, ans);
        }
    }
}

/*
* ����
*/
void Div_Operate(ST_Table* ST, Memory* Mem, string var, string content, char op) {
    int pos_op;//��������λ��
    pos_op = str_match(content, op);
    int right_end;//�µ��Ӵ��ķֺŵ�λ��
    right_end = str_match(content, ';');
    string left = content.substr(0, pos_op);//��Ԫ
    string right = content.substr(pos_op + 1, right_end - pos_op - 1);//��Ԫ
    int is_same_type = is_Same_Type(ST, left, right);
    string type1 = Get_Type(ST, var);
    if (is_same_type == 0) {
        cout << "TypeError: " << "Two Type not same\n";
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
                cout << "TypeError: " << "Three Type not same\n";
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
                cout << "TypeError: " << "Three Type not same\n";
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
                cout << "TypeError: " << "Three Type not same\n";
                return;
            }
        }
        else if (is_same_type == 4) { //string,string
            cout << "TypeError: " << "string don't support " << op << endl;
            return;
        }
        else if (is_same_type == 5) { //string(int),string(int)
            string left_value = Fetch_Value(ST, Mem, left);
            string right_value = Fetch_Value(ST, Mem, right);
            int left_int = atoi(left_value.c_str());
            int right_int = atoi(right_value.c_str());
            int ans = left_int / right_int;
            Modify_Mem_int(ST, Mem, var, ans);
        }
    }
}

/*
* ��ʱ�����ļ���
*/
void Temp_Del_Operate(ST_Table* ST, Memory* Mem, string content, char op) {
    int pos_op = str_match(content, op);
    string left_num = content.substr(0, pos_op);//��Ԫ
    string left = left_num;
    string right_num = content.substr(pos_op + 1, content.length()- pos_op -1);//��Ԫ
    string right = right_num;
    int is_same_type = is_Same_Type(ST, left, right);
    if (is_same_type == 0) {
        cout << "TypeError: " << "Two Type not same\n";
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

            cout << "TypeError: " << "string don't support " << op << endl;
        }
        else if (is_same_type == 5) { //string(int),string(int)
            string left_value = Fetch_Value(ST, Mem, left);
            string right_value = Fetch_Value(ST, Mem, right);
            int left_int = atoi(left_value.c_str());
            int right_int = atoi(right_value.c_str());
            temp = to_string(left_int - right_int);
            cout << temp << endl;
        }
    }
}

/*
* ��ʱ�����ĳ˷�
*/
void Temp_Mul_Operate(ST_Table* ST, Memory* Mem, string content, char op) {
    int pos_op = str_match(content, op);
    string left_num = content.substr(0, pos_op);//��Ԫ
    string left = left_num;
    string right_num = content.substr(pos_op + 1, content.length() - pos_op - 1);//��Ԫ
    string right = right_num;
    int is_same_type = is_Same_Type(ST, left, right);
    if (is_same_type == 0) {
        cout << "TypeError: " << "Two Type not same\n";
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

            cout << "TypeError: " << "string don't support " << op << endl;
        }
        else if (is_same_type == 5) {
            string left_value = Fetch_Value(ST, Mem, left);
            string right_value = Fetch_Value(ST, Mem, right);
            int left_int = atoi(left_value.c_str());
            int right_int = atoi(right_value.c_str());
            temp = to_string(left_int * right_int);
            cout << temp << endl;
        }
    }
}

/*
* ��ʱ�����ĳ���
*/
void Temp_Div_Operate(ST_Table* ST, Memory* Mem, string content, char op) {
    int pos_op = str_match(content, op);
    string left_num = content.substr(0, pos_op);//��Ԫ
    string left = left_num;
    string right_num = content.substr(pos_op + 1, content.length() - pos_op - 1);//��Ԫ
    string right = right_num;
    int is_same_type = is_Same_Type(ST, left, right);
    if (is_same_type == 0) {
        cout << "TypeError: " << "Two Type not same\n";
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

            cout << "TypeError: " << "string don't support " << op << endl;
        }
        else if (is_same_type == 5) {

            string left_value = Fetch_Value(ST, Mem, left);
            string right_value = Fetch_Value(ST, Mem, right);
            int left_int = atoi(left_value.c_str());
            int right_int = atoi(right_value.c_str());
            temp = to_string(left_int / right_int);
            cout << temp << endl;
        }
    }
}

/*
* Int�ļ���,�˷�����������
*/
void Int_OtherOperate(ST_Table* ST, Memory* Mem, string cmd)
{
    int pos = str_match(cmd, '=');
    int pos_end = str_match(cmd, ';');
    if (pos != -1) {
        //�Ҳ�������λ��
        string var = cmd.substr(0, pos);
        string content = cmd.substr(pos + 1, pos_end - pos - 1);
        int pos_del = str_match(cmd, '-');
        int pos_mul = str_match(cmd, '*');
        int pos_div = str_match(cmd, '/');
        if (pos_del != -1) {//����
            Del_Operate(ST, Mem, var, content, '-');
        }
        else if (pos_mul != -1) {//�˷�
            Mul_Operate(ST, Mem, var, content, '*');
        }
        else if (pos_div != -1) {//����
            Div_Operate(ST, Mem, var, content, '/');
        }
        else {
            cout << "Input Operater Invalid!\n";
            return;
        }
    }
    else {//������ʱ����
        string content = cmd.substr(0, cmd.length() - 1);
        int pos_del = str_match(cmd, '-');
        int pos_mul = str_match(cmd, '*');
        int pos_div = str_match(cmd, '/');
        if (pos_del != -1) {//����
            Temp_Del_Operate(ST, Mem, content, '-');
        }
        else if (pos_mul != -1) {//�˷�
            Temp_Mul_Operate(ST, Mem, content, '*');
        }
        else if (pos_div != -1) {//����
            Temp_Div_Operate(ST, Mem, content, '/');
        }
        else {
            cout << "Input Operater Invalid!\n";
            return;
        }
    }
}

/*
* ��String_Compare��ʹ��
*/
void Compare_String(ST_Table* ST, Memory* Mem, string left_value, string right_value,char op) {
    string ans;
    int len = left_value.length() > right_value.length() ? left_value.length() : right_value.length();
    int left_len = left_value.length();
    int right_len = right_value.length();
    if (op == '|') {
        //��ʼ�Ƚ�
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
        //��ʼ�Ƚ�
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
* ȥ��string�е����пո�
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
* string��λȡ���ȡС
*/
void String_Compare(ST_Table* ST, Memory* Mem, string cmd)
{
    int pos_big = str_match(cmd, '|');
    int pos_small = str_match(cmd, '^');
    if (pos_big != -1) {//ȡ��
        string left = cmd.substr(0, pos_big);//��Ԫ
        string right = cmd.substr(pos_big + 1, cmd.length() - pos_big - 2);//��Ԫ
        int l_f = Variable_Find(ST, left);
        int r_f = Variable_Find(ST, right);
        if (l_f != -1 && r_f != -1) {//string����+string����
            string left_value;
            string right_value;
            left_value = Fetch_Value(ST, Mem, left);
            right_value = Fetch_Value(ST, Mem, right);
            Compare_String(ST, Mem, left_value, right_value,'|');
            
        }
        else if (l_f != -1 && r_f == -1) {//����+string����
            string left_value = Fetch_Value(ST, Mem, left);
            string right_value = right;
            Compare_String(ST, Mem, left_value, right_value, '|');
        }
        else if (l_f == -1 && r_f != -1) {//string����+����
            string left_value = left;
            string right_value = Fetch_Value(ST, Mem, right);
            Compare_String(ST, Mem, left_value, right_value, '|');
        }
        else {
            cout << "Type Wrong\n";
            return;
        }
    }
    else if (pos_small != -1) {//ȡС
        string left = cmd.substr(0, pos_small);//��Ԫ
        string right = cmd.substr(pos_small + 1, cmd.length() - pos_small - 2);//��Ԫ
        int l_f = Variable_Find(ST, left);
        int r_f = Variable_Find(ST, right);
        if (l_f != -1 && r_f != -1) {//string����+string����
            string left_value;
            string right_value;
            left_value = Fetch_Value(ST, Mem, left);
            right_value = Fetch_Value(ST, Mem, right);
            Compare_String(ST, Mem, left_value, right_value, '^');

        }
        else if (l_f != -1 && r_f == -1) {//����+string����
            string left_value = Fetch_Value(ST, Mem, left);
            string right_value = right;
            Compare_String(ST, Mem, left_value, right_value, '^');
        }
        else if (l_f == -1 && r_f != -1) {//string����+����
            string left_value = left;
            string right_value = Fetch_Value(ST, Mem, right);
            Compare_String(ST, Mem, left_value, right_value, '^');
        }
        
        else {
            cout << "TypeError: " << "Type Wrong\n";
            return;
        }
    }
    else {
        cout << "Syntax Error: " << "Input Invalid!\n";
        return;
    }
}


/*
* ���庯��
*/
void Fun_Define(ST_Table* ST, Memory* Mem, string cmd)
{
    string fun_name;
    trim(cmd);
    int pos_end = str_match(cmd, ';');
    fun_name = cmd.substr(6, pos_end - 6);

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
    string fun_body;//����������
    string statement;
    string fun_body_noback;//�޻��еĺ�����

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
    //��returntype��argutype��д��ST�У���fun_body����Mem��
    Judge_Fun_Repeat(ST, Mem,type ,fun_name); //���ST�����д
    Fun_Assignment(ST, Mem, fun_name, type, fun_body);


    cout << ">>>";
    cout << "Fun body:" << endl;
    cout << fun_body;
    return;
}

/*
* ����if���,�жϵ�����Ϊa>b
* if a>b:
*   b=a;
* else:
*   a=b;
* endif
*/
void If_Operate(ST_Table* ST, Memory* Mem, string cmd)
{
    trim(cmd);
    int pos_big = str_match(cmd, '>');

    int pos_mh = str_match(cmd, ':');
    if (pos_mh == -1) {
        cout << "SyntaxError: invalid syntax!\n";
        return;
    }
    if (pos_big == -1) {
        cout << "SyntaxError: Only '>' Supported!\n";
        return;
    }
    string var1 = cmd.substr(2, pos_big - 2); //��Ԫ
    string var2 = cmd.substr(pos_big + 1, pos_mh - pos_big - 1); //��Ԫ
    string value1 = Fetch_Value(ST, Mem, var1);
    string value2 = Fetch_Value(ST, Mem, var2);
    if (value1 == "" || value2 == "") {
        cout << "ValueError: Please Input the Variable have been Declared!\n";
        return;
    }
    int value_a= atoi(value1.c_str());
    int value_b = atoi(value2.c_str());

    cout << "..";
    string statement1;
    getline(cin, statement1); //b=a;

    cout << "..";
    string else_state;
    getline(cin, else_state); //else:

    cout << "..";
    string statement2;      //a=b;
    getline(cin, statement2);

    cout << "..";
    string end_if;
    getline(cin, end_if);//endif;

    //��ʼ��������
    if (value_a > value_b) {
        Main_Switch(ST, Mem, statement1);
    }
    else {
        Main_Switch(ST, Mem, statement2);
    }
}

void Main_Switch(ST_Table*ST,Memory*Mem,string cmd) {
    if (cmd.substr(0, 3) == "int" || cmd.substr(0, 6) == "string") {
        Declare_Variable(cmd, ST, Mem);
    }
    else if (str_match(cmd, '=') != -1 && str_match(cmd, '+') == -1 && str_match(cmd, ';') != -1 && str_match(cmd, '-') == -1
        && str_match(cmd, '*') == -1 && str_match(cmd, '/') == -1) {
        Variable_Assignment(ST, Mem, cmd);
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
    else if (str_match(cmd, '|') != -1 || str_match(cmd, '^') != -1) {
        String_Compare(ST, Mem, cmd);
    }
    else if (str_match(cmd, '-') != -1 || str_match(cmd, '*') != -1 || str_match(cmd, '/') != -1) {
        Int_OtherOperate(ST, Mem, cmd);
    }
    else if (cmd.substr(0, 6) == "define") {
        Fun_Define(ST, Mem, cmd);
    }
    else if (cmd.substr(0, 4) == "call") {

    }
    else if (cmd.substr(0, 2) == "if") {
        If_Operate(ST, Mem, cmd);
    }
    else if (cmd.substr(0, 6) == "return") {
        string value=Return_Operate(ST, Mem, cmd);
    }

}


/*
* �����Ǻ������أ�ÿ������������һ��
*/
int Fun_Find_NoType(ST_Table* ST, Memory* Mem, string fun_name) {
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name == fun_name && (ST[i]->Var_type != "int" || ST[i]->Var_type != "string")) {
            return 1;
        }
    }
    return -1;
}

/*����������'\n'�ָ�*/
vector<string> Fun_Split(string s, const string& seperator) {
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;

    while (i != s.size()) {
        //�ҵ��ַ������׸������ڷָ�������ĸ��
        int flag = 0;
        while (i != s.size() && flag == 0) {
            flag = 1;
            for (string_size x = 0; x < seperator.size(); ++x)
                if (s[i] == seperator[x]) {
                    ++i;
                    flag = 0;
                    break;
                }
        }

        //�ҵ���һ���ָ������������ָ���֮����ַ���ȡ����
        flag = 0;
        string_size j = i;
        while (j != s.size() && flag == 0) {
            for (string_size x = 0; x < seperator.size(); ++x)
                if (s[j] == seperator[x]) {
                    flag = 1;
                    break;
                }
            if (flag == 0)
                ++j;
        }
        if (i != j) {
            result.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return result;
}

/*
* ��һ���ַ����е�ָ�������滻Ϊ��һ������
*/
void replace_all_ditinct(string& str, const string& old_value, const string& new_value)
{
    for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != string::npos)
            str.replace(pos, old_value.length(), new_value);
        else break;
    }
}

/*
* ��������,���޶�Ԫ����
* cmd:call Fun(a,b);
*/
void Fun_Call(ST_Table* ST, Memory* Mem, string cmd)
{
    
    trim(cmd);
    int pos_left = str_match(cmd, '(');//������
    int pos_right = str_match(cmd, ')');//������
    int pos_douhao = str_match(cmd, ',');
    int pos_end = str_match(cmd, ';');
    string fun_name = cmd.substr(4, pos_left - 4);//������
    string var1 = cmd.substr(pos_left + 1, pos_douhao - pos_left - 1);//��һ������
    string var2 = cmd.substr(pos_douhao + 1, pos_right - pos_douhao - 1);//�ڶ�������
    
    //������û�ж����������
    int is_exist=Fun_Find_NoType(ST, Mem, var1);
    int temp_var = 0;
    if (is_exist == -1) {
        cout << "Error: " << fun_name << " not defined!\n";
    }
    else {
        string cmd_list = Fetch_Value(ST, Mem, fun_name);

        //�Ȱ��β��滻Ϊʵ��
         //�Ȼ�øú����Ĳ����б�
        string type = Get_Type(ST, fun_name);
        //�Ѳ����б�fun(int a,int b,int)���,�õ�a,b������������
        int pos_first_douhao = str_match(type, ',');//�õ���һ�����õ�λ��
        int pos_first_space = str_match(type, ' ');//�õ���һ���ո��λ��
        int pos_right = str_match(type, ')');
        string a = type.substr(pos_first_space + 1, pos_first_douhao - pos_first_space - 1);//��һ��������
        string sub_type = type.substr(pos_first_douhao + 1, pos_right - pos_douhao - 1);//�Ӵ�

        int pos_second_douhao = str_match(sub_type, ',');
        int pos_second_space = str_match(sub_type, ' ');
        string b = sub_type.substr(pos_second_space + 1, pos_second_douhao - pos_second_space - 1);


        vector<string> cmd_vec = Fun_Split(cmd_list,"\n");//���ָ��ķָ�
        for (vector<string>::size_type i = 0; i != cmd_vec.size(); ++i)
        {
            if (cmd_vec[i].substr(0, 3) == "int" || cmd_vec[i].substr(0, 6) == "string")
                temp_var++; //��ʱ�����ĸ���

            replace_all_ditinct(cmd_vec[i], a, var1);
            replace_all_ditinct(cmd_vec[i], b, var2);//�β��滻Ϊʵ��
            Main_Switch(ST,Mem,cmd_vec[i]);

        }
        //��ST�����ʱ����ɾ��
        Remove_var(ST, Mem, temp_var);
    }

}

/*
* �������е�return���
*/
string Return_Operate(ST_Table*ST, Memory* Mem, string cmd)
{
    trim(cmd);
    int pos_end = str_match(cmd, ';');
    string var_name = cmd.substr(6, pos_end - 6);
    string value = Fetch_Value(ST, Mem, var_name);
    return value;
}

/*
* ���ST���Mem��ǰ�ĳ���
*/
int ST_Mem_Len(ST_Table* ST, Memory* Mem) {
    int len = 0;
    for (int i = 0; i < ST_Length; i++) {
        if (ST[i]->Var_name != "") {
            len++;
        }
        else {
            break;
        }
    }
    return len;
}

/*
* ����ִ����������ʱ����
*/
void Remove_var(ST_Table* ST, Memory* Mem, int temp_num)
{
    int f = 0;
    int len = ST_Mem_Len(ST, Mem);
    for (int i = len-temp_num; i < len; i++) {
        int addr = ST[i]->address;
        ST[i]->Var_name = "";
        ST[i]->Var_type = "";
        ST[i]->address = 0;

        for (int j = len - temp_num; j < len; j++) {
            if (j == addr) {
                Mem[j]->content = "";
                Mem[j]->flag = 0;
                break;
            }
        }
    }
}


int main() {
    ST_Table* ST;
    Memory* Mem;
    ST = new ST_Table[ST_Length];
    Mem = new Memory[Mem_Length];
   

    Init_ST(ST);
    Init_Mem(Mem);
    cout << "U Lang 1.0 @Author:YouYuanYi\nType Help or help for more information.\n";
    while (1) {
        cout << ">>>";
        string cmd = "";
        getline(cin, cmd);
        if (cmd.substr(0,3)=="int" || cmd.substr(0,6)=="string") {
            Declare_Variable(cmd, ST, Mem);
        }
        else if (str_match(cmd, '=') != -1 && str_match(cmd, '+') == -1 && str_match(cmd, ';') != -1 && str_match(cmd, '-') == -1
            && str_match(cmd, '*') == -1 && str_match(cmd, '/') == -1) {
            Variable_Assignment(ST, Mem, cmd);
        }
        else if (cmd == "exit;") {
            cout << "Bye!\n";
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
            string value = Get_Value(ST, Mem, cmd);
            if(value!="")
             cout <<value<< endl;
        }
        else if (str_match(cmd, '+') != -1) {
            Variable_Add(ST, Mem, cmd);
        }
        else if (cmd.substr(0, 4) == "len(") {
            int len = Get_String_Len(ST, Mem, cmd);
            if (len != 0 && len!=-1) {
                cout << len << endl;
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
        else if (cmd.substr(0, 2) == "if") {
            If_Operate(ST, Mem, cmd);
        }
        else if (cmd.substr(0, 4) == "call") {
            Fun_Call(ST, Mem, cmd);
        }
        else if (cmd == "help" || cmd=="Help") {
            Help();
        }
        else if (cmd.substr(0,5) == "clear") {
            system("cls");
        }
    }
    delete [] ST;
    delete [] Mem;
    return 0;
}