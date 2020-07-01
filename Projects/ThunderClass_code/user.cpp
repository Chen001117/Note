#include "user.h"
#include "admin.h"
#include "teacher.h"
#include "student.h"

//用户总数
unsigned int User::m_uUserCount = 0;
//用户总数常引用
const unsigned int& User::UserCount = User::m_uUserCount;
//用户列表
vector<User*> User::m_UserList;
//最新登入用户
User* User::m_LoginedUser = nullptr;


/*************************************************************************
【函数名称】User::User
【函数功能】构造函数
【参数】
    Name 输入参数，使用者姓名
    Password 输入参数，使用者密码
    Type 输入参数，使用者类型
【返回值】构造函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
User::User(const string& Name, const string& Password, const string& Type):Type(m_sType){
    m_sName = Name;
    m_sPassword = Password;
    m_sType = Type;
    m_uUserCount++;
}

/*************************************************************************
【函数名称】User::User
【函数功能】构造函数
【参数】inFile 输入参数，源文件对象
【返回值】构造函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
User::User(ifstream& inFile):Type(m_sType){
    getline(inFile, m_sName);
    getline(inFile, m_sPassword);
    getline(inFile, m_sType);
    m_uUserCount++;
}

/*************************************************************************
【函数名称】User::~User
【函数功能】析构函数
【参数】无
【返回值】无
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
User::~User(){
    m_uUserCount--;
}

/*************************************************************************
【函数名称】User::User
【函数功能】拷贝构造函数
【参数】anUser 输入参数，表源对象
【返回值】构造函数不可有返回值
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
User::User(const User& anUser):Type(m_sType){
    m_sName = anUser.m_sName;
    m_sPassword = anUser.m_sPassword;
    m_sType = anUser.m_sType;
}

/*************************************************************************
【函数名称】operator=
【函数功能】赋值运算符
【参数】anUser 输入参数，表源对象
【返回值】User类对象的引用
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
User& User::operator=(const User& anUser){
    if (this != &anUser) {
        m_sName = anUser.m_sName;
        m_sPassword = anUser.m_sPassword;
        m_sType = anUser.m_sType;
    }
    return *this;
}

/*************************************************************************
【函数名称】User::ToMessage
【函数功能】将用户转换为消息类对象
【参数】无
【返回值】Message类对象
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
Message User::ToMessage() const {
    unsigned int NameLen = m_sName.length();
    unsigned int PasswordLen = m_sPassword.length();
    unsigned char* Buffer = new unsigned char[NameLen + PasswordLen + 2 * sizeof (NameLen)];
    memcpy(Buffer, (char*)&NameLen, sizeof(NameLen));
    memcpy(Buffer + sizeof(NameLen), m_sName.c_str(), NameLen);
    memcpy(Buffer + sizeof(NameLen) + NameLen, (char*)&PasswordLen, sizeof(PasswordLen));
    memcpy(Buffer +  2 * sizeof(NameLen) + NameLen, m_sPassword.c_str(), PasswordLen);
    Message ret(MSG_ID_PSW, Buffer, NameLen + PasswordLen + 2 * sizeof (NameLen));
    delete [] Buffer;
    return ret;
}

/*************************************************************************
【函数名称】User::SaveToFileStream
【函数功能】将用户信息存入文件
【参数】OutFile 输入参数，表示要存入的文件变量
【返回值】Message类对象
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
void User::SaveToFileStream(ofstream& OutFile) const {
    OutFile << m_sName << endl;
    OutFile << m_sPassword << endl;
    OutFile << m_sType << endl;
}

/*************************************************************************
【函数名称】User::GetName
【函数功能】得到用户名
【参数】无
【返回值】使用者名称string
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
string User::GetName() const{
    return m_sName;
}

/*************************************************************************
【函数名称】User::TestPassword
【函数功能】测试密码是否正确
【参数】Password 输入参数，待测试密码
【返回值】若密码正确则返回true，错误返回false
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
bool User::TestPassword(const string& Password) const {
    if (Password == m_sPassword) {
        return true;
    }
    else {
        return false;
    }
}

/*************************************************************************
【函数名称】User::AddUser
【函数功能】在用户列表中添加用户
【参数】
    Name 输入参数，使用者姓名
    Password 输入参数，使用者密码
    Type 输入参数，使用者类型
【返回值】添加成功返回true，否则false
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
bool User::AddUser(const string& Name, const string& Password, const string& Type){
    Admin* p = nullptr;
    p = dynamic_cast<Admin*>(this);
    if (p != nullptr) {
        if (Type == "Admin") {
            for (unsigned int i  = 0; i < m_UserList.size(); i++) {
                if (m_UserList[i]->Type == "Admin") {
                    return false;
                }
            }
            m_UserList.push_back(new Admin());
            return true;
        }
        else if (Type == "Teacher"){
            m_UserList.push_back(new Teacher(Name, Password));
            return true;
        }
        else {
            m_UserList.push_back((User*) new Student(Name, Password));
            return true;
        }
    }
    else {
        return false;
    }
}

/*************************************************************************
【函数名称】User::delUser
【函数功能】在用户列表中删除用户
【参数】
    Name 输入参数，使用者姓名
    Password 输入参数，使用者密码
    Type 输入参数，使用者类型
【返回值】删除成功返回true，否则false
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
bool User::delUser(const string& Name, const string& Password, const string& Type){
    Admin* p = nullptr;
    p = dynamic_cast<Admin*>(this);
    if (p != nullptr) {
        //判断类型是否正确
        if(Type == "Teacher" || Type == "Student"){
            //判断是否存在该用户名
            const User* aUser = User::GetUser(Name);
            if(aUser != nullptr){
                //判断用户密码与类型是否相符
                if(aUser->TestPassword(Password)==true && aUser->Type == Type){
                    for(unsigned int i = 0; i < m_UserList.size(); i++){
                        if(m_UserList[i]->GetName() == Name){
                            m_UserList.erase(m_UserList.begin() + i);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
/*************************************************************************
【函数名称】User::LoadFromFile
【函数功能】用文件内容更新用户列表
【参数】FileName 输入参数，表示源文件名
【返回值】无
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】
    2020-05-19 范静涛增加了未找到文件则自动创建只包含Admin用户的功能
    2020-05-23 范静涛修改了重复调用此函数导致用户数翻倍的问题，感谢李浦豪同学发现此问题
    2020-06-21 由陈文泽增加注释
*************************************************************************/
void User::LoadFromFile(const string& FileName) {
    ifstream inFile(FileName.c_str());
    if (inFile.fail()) {
        ofstream OutFile(FileName.c_str(), ios::out | ios::app);
        OutFile << 1 << endl << "Admin" << endl << "Admin" << endl << "Admin" << endl;
        OutFile.close();
    }
    inFile.close();
    inFile.open(FileName.c_str());

    //2020-05-23 新增开始
    for (int i = m_UserList.size() - 1; i >= 0 ; i--) {
        User* pUser = m_UserList[i];
        delete pUser;
    }
    //2020-05-23 新增结束
    m_UserList.clear();
    unsigned int UserCount;
    inFile >> UserCount;
    inFile.get();
    for (unsigned int i  = 0; i < UserCount; i++) {
        string Name;
        string Password;
        string Type;
        getline(inFile, Name);
        getline(inFile, Password);
        getline(inFile, Type);
        if (Type == "Admin"){
            m_UserList.push_back(new Admin());
        }
        else if (Type == "Teacher"){
            m_UserList.push_back(new Teacher(Name, Password));
        }
        else {
            m_UserList.push_back((User*)new Student(Name, Password));
        }
    }
    inFile.close();
}

/*************************************************************************
【函数名称】User::SaveToFile
【函数功能】用用户列表更新文件
【参数】FileName 输入参数，表示源文件名
【返回值】无
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
void User::SaveToFile(const string& FileName){
    ofstream OutFile(FileName.c_str());
    OutFile << m_UserList.size() << endl;
    for (unsigned int i  = 0; i < m_UserList.size(); i++) {
        m_UserList[i]->SaveToFileStream(OutFile);
    }
}

/*************************************************************************
【函数名称】User::GetUser
【函数功能】用账号密码找到用户
【参数】
    Name 输入参数，用户名
    Password 输入参数，用户密码
【返回值】User类指针
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
const User* User::GetUser(const string& Name, const string& Password){
    const User* Current = nullptr;
    for (unsigned int i  = 0; i < m_UserList.size(); i++) {
        if (m_UserList[i]->GetName() == Name && m_UserList[i]->TestPassword(Password)) {
            Current = m_UserList[i];
            m_LoginedUser = m_UserList[i];
            break;
        }
    }
    return Current;

}

/*************************************************************************
【函数名称】User::GetUser
【函数功能】用账号找到用户
【参数】Name 输入参数，用户名
【返回值】User类指针
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
const User* User::GetUser(const string& Name) {
    const User* Current = nullptr;
    for (unsigned int i  = 0; i < m_UserList.size(); i++) {
        if (m_UserList[i]->GetName() == Name) {
            Current = m_UserList[i];
            break;
        }
    }
    return Current;

}

/*************************************************************************
【函数名称】User::GetLoginedUser()
【函数功能】得到最新登入用户
【参数】无
【返回值】User类指针
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
User* User::GetLoginedUser() {
    return m_LoginedUser;
}

/*************************************************************************
【函数名称】User::OfflineAllStudent
【函数功能】将所有学生下线
【参数】无
【返回值】无
【开发者及日期】范静涛(fanjingtao@tsinghua.edu.cn) 2020-5-3
【更改记录】2020-06-21 由陈文泽增加注释
*************************************************************************/
void User::OfflineAllStudents(){
    Student* pStudent;
    for (unsigned int i  = 0; i < m_UserList.size(); i++) {
        if (m_UserList[i]->Type == "Student") {
            pStudent = dynamic_cast<Student*>(m_UserList[i]);
            pStudent->Offline();
        }
    }
}
