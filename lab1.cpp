#include<iostream>
#include<cstring>
#include<string>
#include<cstdlib>
#include<math.h>
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
void Declare_Variable(string type, ST_Table* ST, Memory* Mem) { //��������
    string var_name; //�����ı�����
    cin >> var_name;
    int end_with_fh = str_match(var_name, ';'); //�ж��Ƿ��ԷֺŽ�β
   if (end_with_fh == -1){
        cout << "Input Invalid!\n";
        return;
   }
   else {
       var_name = var_name.substr(0, var_name.length() - 1);
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
        cout << var << " has existed!\n";
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
        cout << var << " has existed\n";
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
        cout << "Please declare " << var << " first!\n" << endl;
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
* ��ñ�����ֵ:$a;
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
            else //���Ͳ�����
            {
                return 0;//������string
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
            else if(right_type=="")
                return 5;
           
        }
        else {//��Ԫ����Ϊint
            if (is_Int(right) == 1) { //��(string)����������
                return 3;
            }
            else { //��Ԫ����Ϊstring
                string right_type = Get_Type(ST, right);//��Ԫ��type
                if (right_type == "int") { //��Ԫ�����ֵΪint
                    return 2;
                }
                else { //��int��string
                    return 0;
                }
            }
        }

    }
}

/*
* ����var_name��ֵ
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
    else {//��ʾ��ʱ����temp
        int pos_add = str_match(cmd, '+');
        left_num = cmd.substr(0, pos_add);//��Ԫ
        string left = left_num;
        right_num = cmd.substr(pos_add + 1, pos_end - pos_add - 1);//��Ԫ
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
* ���庯��
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