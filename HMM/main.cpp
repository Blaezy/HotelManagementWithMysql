#include <iostream>
#include <string.h>
#include <conio.h>
#include<windows.h>
#include<mysql.h>
#include <sstream>
#define MAX 100
using namespace std;

    int qstate;
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;


class Customer
{
    public:
    string name;
    string address;
    string phone;
    string fdate;
    string tdate;
    string advance_payment;
    string booking_id;

};

class Room
{
public:
    char type;
    char stype;
    char ac;
    string roomnumber;
    string rent;
    string status;

    class Customer cust;
    void addRoom(string);
    void searchRoom(string );
    void deleteRoom(string);
    void displayRoom();
};
class Room rooms[MAX];
int count1=4;


void Room::addRoom(string roomNo)
{
    class Room room;
    room.roomnumber=roomNo;
    string dummy;
    cout<<"\nType AC/Non-AC (A/N) : ";
    cin>>room.ac;
    cout<<"\nType Comfort (S/N) : ";
    cin>>room.type;
    cout<<"\nType Size (B/S) : ";
    cin>>room.stype;
    cout<<"\nDaily Rent : ";
    cin>>room.rent;
    room.status="0";
    stringstream ss;

    conn=mysql_init(0);
    conn=mysql_real_connect(conn,"192.168.137.1","admin","admin","HMM",0,NULL,0);
    if(conn)
    {
        qstate=0;
        ss << "INSERT INTO room_dt(type, stype, AC,roomnumber,rent,status) VALUES('" << room.type << "','" << room.stype << "','" << room.ac <<"','" << room.roomnumber <<"','" << room.rent <<"','" << room.status <<"')";
        string query = ss.str();
        const char* q = query.c_str();
        qstate = mysql_query(conn, q);
        if(qstate == 0){
            cout << "Record Inserted..." << endl;
            cout<<"\n Room Added Successfully!";
            getch();
        }else{
            cout << "Insert Error" << mysql_error(conn) << endl;
            cin >> dummy;

        }
    }
}





void Room::displayRoom()
{

    cout<<"\nRoom Number: \t"<<row[3];
    cout<<"\nType AC/Non-AC (A/N) "<<row[2];
    cout<<"\nType Comfort (S/N) "<<row[0];
    cout<<"\nType Size (B/S) "<<row[1];
    cout<<"\nRent: "<<row[4];
}


void Room::searchRoom(string roomNo)
{

    int c=0;

    string findbyname_query = "select * from  room_dt where roomnumber = '"+roomNo+"'";
    const char* qn = findbyname_query.c_str();
    qstate = mysql_query(conn, qn);

    if (!qstate)
    {
        res = mysql_store_result(conn);
        while ((row = mysql_fetch_row(res)))
        {
            cout<<"Room Details\n";
            if(stoi(row[5])==1)
            {
                cout<<"\nRoom is Reserved";
            }
            else
            {
                cout<<"\nRoom is available";
            }
            displayRoom();
            getch();
            c++;
        }

        if (c == 0) {
            cout << "No Room with this input" << endl;
            getch();
        }
    }
    else
    {
        cout << "Query Execution Problem!" << mysql_errno(conn) << endl;
    }

}
class HMM:protected Room
{
public:
    void checkIn();
    void getAvailRoom();
    void searchCustomer(string);
    void checkOut();
    void guestSummaryReport();
};

void HMM::guestSummaryReport()
{
    string query="select *from customer_dt";
    const char *q=query.c_str();
    qstate=mysql_query(conn,q);
    if(!qstate)
    {
        res=mysql_store_result(conn);
        my_ulonglong x=mysql_num_rows(res);
        if(x<1)
        {
             cout<<"\n NO guest in Hotel!!";
        }


        while((row=(mysql_fetch_row(res))))
        {
            cout<<"\n Customer First Name : "<<row[0];
            cout<<"\n Room Number : "<<row[7];
            cout<<"\n Address (only city) : "<<row[1];
            cout<<"\n Phone : "<<row[2];
            cout<<"\n---------------------------------------";
        }


    }




    /*if(count1==0)
    {
        cout<<"\n NO guest in Hotel!!";
    }
    for(int i=0;i<count1;i++)
    {
        if(rooms[i].status=="1")
        {
            cout<<"\n Customer First Name : "<<rooms[i].cust.name;
            cout<<"\n Room Number : "<<rooms[i].roomnumber;
            cout<<"\n Address (only city) : "<<rooms[i].cust.address;
            cout<<"\n Phone : "<<rooms[i].cust.phone;
            cout<<"\n---------------------------------------";
        }

    }*/

    getch();
};

void HMM::checkIn()
{
    int found=0;
    string rno;
    string dummy;

    class Room room;
    cout<<"\nEnter Room number : ";
    cin>>rno;
        string findbyname_query = "select * from  room_dt where roomnumber = '"+rno+"'";
        const char*qn=findbyname_query.c_str();
        qstate=mysql_query(conn,qn);


    if(!qstate)
    {
        res=mysql_store_result(conn);
        while((row=mysql_fetch_row(res)))
        {
            if(row[3]==rno)
            {
                found=1;
               break;
            }
        }
        if(found==1)
        {
            if(stoi(row[5])==1)
        {
            cout<<"\nRoom is already Booked";
            getch();
            return;
        }
        }

    }
    /*for(i=0;i<count1;i++)
    {
    if(rooms[i].roomnumber==rno)
    {
    found=1;
    break;
    }
    }
    if(found==1)
    {
    if(rooms[i].status=="1")
    {
        cout<<"\nRoom is already Booked";
        getch();
        return;
    }*/
        my_ulonglong x=mysql_num_rows(res);
        if(x>=1)
        {
           cout<<"\nEnter booking id: ";
            cin>>room.cust.booking_id;

            cout<<"\nEnter Customer Name (First Name): ";
            cin>>room.cust.name;

            cout<<"\nEnter Address (only city): ";
            cin>>room.cust.address;

            cout<<"\nEnter Phone: ";
            cin>>room.cust.phone;

            cout<<"\nEnter From Date: ";
            cin>>room.cust.fdate;

            cout<<"\nEnter to  Date: ";
            cin>>room.cust.tdate;


            cout<<"\nEnter Advance Payment: ";
            cin>>room.cust.advance_payment;

            room.status="1";
            string updatQuery = "update room_dt set status='"+room.status+"' where roomnumber = '" +rno+ "'";
            const char* un = updatQuery.c_str();
            qstate = mysql_query(conn, un);


            stringstream ss;
            if(conn)
            {
                qstate=0;
                ss << "INSERT INTO customer_dt(name, address,phone, fdate,tdate,advance_payment,booking_id,room_no) VALUES('" << room.cust.name << "','" << room.cust.address << "','" << room.cust.phone <<"','" << room.cust.fdate <<"','" << room.cust.tdate <<"','" << room.cust.advance_payment <<"','" << room.cust.booking_id <<"','" << rno << "')";
                string query=ss.str();
                const char* q=query.c_str();
                qstate=mysql_query(conn,q);
                if(qstate==0)
                {
                    cout << "Record Inserted..." << endl;
                }
                else{
                cout << "Insert Error" << mysql_error(conn) << endl;
                cout << "Press B to go back";
                cin >> dummy;
            }
            }

            cout<<"\n Customer Checked-in Successfully..";
            getch();

        }
        else
            {
                cout<<"NO such Room exists";
                getch();
            }

    }


void HMM::getAvailRoom()
{
    int found=0;
    string av="0";
    string getAvailabkeRoomQuery="select * from  room_dt where status = '"+av+"'";
    const char*qn=getAvailabkeRoomQuery.c_str();
    qstate=mysql_query(conn,qn);

    if(!qstate)
    {
        res=mysql_store_result(conn);
        while((row=mysql_fetch_row(res)))
        {
            displayRoom();
            cout<<"\n\nPress enter for next room";
            found=1;
            getch();
        }
        if(found==0)
        {
            cout<<"\nAll rooms are reserved";
            getch();

        }

    }



    /*for(i=0;i<count1;i++)
    {
    if(rooms[i].status=="0")
    {
        displayRoom();
        cout<<"\n\nPress enter for next room";
        found=1;
        getch();
    }
    }
    if(found==0)
    {
        cout<<"\nAll rooms are reserved";
        getch();
    }*/

}

void HMM::searchCustomer(string pname)
{
    int found=0;
        string findbyname_query = "select * from  customer_dt where name = '"+pname+"'";
        const char*qn=findbyname_query.c_str();
        qstate=mysql_query(conn,qn);

    if(!qstate)
    {
        res=mysql_store_result(conn);
        while((row=(mysql_fetch_row(res))))
        {
            cout<<"\nCustomer Name: "<<row[0];
            cout<<"\nRoom Number: "<<row[7];

            cout<<"\n\nPress enter for next record";
            found=1;
            getch();
        }
    }
    if(found==0)
    {
        cout<<"\nPerson not found.";
        getch();
    }
}

void HMM::checkOut()
{
    int found=0,days;
    string rno;
    int advance,rent;
    float billAmount=0;
    cout<<"Enter Room Number : ";
    cin>>rno;
    string checkoutQuery="select * from  customer_dt where room_no = '"+rno+"'";
    const char *qn=checkoutQuery.c_str();
    qstate=mysql_query(conn,qn);

    if(!qstate)
    {
        res=mysql_store_result(conn);
        while((row=mysql_fetch_row(res)))
        {
            found=1;
            break;
        }
        if(found)
        {

            cout<<"\nEnter Number of Days:\t";
            cin>>days;
            advance=stoi(row[5]);
            billAmount=days * stoi(row[5]);

            cout<<"\n\t######## CheckOut Details ########\n";
            cout<<"\nCustomer Name : "<<row[0];
            cout<<"\nRoom Number : "<<row[7];
            cout<<"\nAddress : "<<row[1];
            cout<<"\nPhone : "<<row[2];
            cout<<"\nAdvance Paid: "<<row[5]<<" /";

        }
    }
    string query="select * from room_dt where roomnumber='"+rno+"'";
    const char *q=query.c_str();
    qstate=mysql_query(conn,q);
    if(!qstate)
    {

        res=mysql_store_result(conn);
        row=mysql_fetch_row(res);
        rent=stoi(row[4]);


    }
    billAmount=rent*days;  //cout<<"\n*** Total Payable: "<<billAmount-stoi(rooms[i].cust.advance_payment)<<"/ only";
    cout<<"\nTotal Amount Due : "<<billAmount-advance<<" /";

    string l="0";
    string query2 = "update room_dt set status='"+l+"' where roomnumber = '" +rno+ "'";
    const char*q1=query2.c_str();
    mysql_query(conn,q1);

    my_ulonglong x=mysql_num_rows(res);
    if(x>=1)
    {
        string query="Delete from customer_dt where room_no='"+rno+"'";
        const char*q=query.c_str();
        qstate=mysql_query(conn,q);
        if(!qstate)
        {
            cout<<"Record Deleted";
        }
    }else
    {
        cout<<"Record deletion unsuccessfull"<<endl;
    }

    getch();
//rooms[i].status="0";
    }


/*if(found==1)
{
cout<<"\nEnter Number of Days:\t";
cin>>days;
billAmount=days * stoi(rooms[i].rent);

cout<<"\n\t######## CheckOut Details ########\n";
cout<<"\nCustomer Name : "<<rooms[i].cust.name;
cout<<"\nRoom Number : "<<rooms[i].roomnumber;
cout<<"\nAddress : "<<rooms[i].cust.address;
cout<<"\nPhone : "<<rooms[i].cust.phone;
cout<<"\nTotal Amount Due : "<<billAmount<<" /";
cout<<"\nAdvance Paid: "<<rooms[i].cust.advance_payment<<" /";
cout<<"\n*** Total Payable: "<<billAmount-stoi(rooms[i].cust.advance_payment)<<"/ only";

rooms[i].status="0";
}*/
//getch();
//}

void manageRooms()
{
    class Room room;
    int opt;
    string rno;
    do
    {
    system("cls");
    cout<<"\n### Manage Rooms ###";
    cout<<"\n1. Add Room";
    cout<<"\n2. Search Room";
    cout<<"\n3. Back to Main Menu";
    cout<<"\n\nEnter Option: ";
    cin>>opt;


    //switch statement
    switch(opt)
    {
    case 1:{
        cout<<"\nEnter Room Number: ";
    cin>>rno;
    string query1="select *from room_dt where roomnumber='"+rno+"'";
    const char *q=query1.c_str();
    mysql_query(conn,q);
    res=mysql_store_result(conn);
    my_ulonglong x=mysql_num_rows(res);
    if(x>=1)
    {
        cout<<"\nRoom Number is Present.Please enter unique Number";
        getch();
    }
    else
    {
        room.addRoom(rno);
        count1++;
    }

    }

    break;
    case 2:
    cout<<"\nEnter room number: ";
    cin>>rno;
    room.searchRoom(rno);
    break;
    case 3:
    //nothing to do
    break;
    default:
    cout<<"\nPlease Enter correct option";
    getch();
    break;
    }
    }while(opt!=3);
}


int main()
{

    conn=mysql_init(0);
    conn=mysql_real_connect(conn,"192.168.137.1","admin","admin","HMM",0,NULL,0);
    if(conn)
    {
        cout<<"Connected";
    }
   class HMM hm;
   int opt;
   string rno;
   string pname;
   system("cls");

do
{
system("cls");
cout<<"######## Hotel Management #########\n";
cout<<"\n1. Manage Rooms";
cout<<"\n2. Check-In Room";
cout<<"\n3. Available Rooms";
cout<<"\n4. Search Customer";
cout<<"\n5. Check-Out Room";
cout<<"\n6. Guest Summary Report";
cout<<"\n7. Exit";
cout<<"\n\nEnter Option: ";
cin>>opt;
switch(opt)
{
case 1:
manageRooms();
break;
case 2:
    {

        string query3="select *from room_dt";
        const char *q=query3.c_str();
        mysql_query(conn,q);
        res=mysql_store_result(conn);
        my_ulonglong x=mysql_num_rows(res);
        if(x<1)
    {
        cout<<"\nRooms data is not available.\nPlease add the rooms first.";
        getch();
    }
        else
        hm.checkIn();

    }




break;
case 3:
if(count1==0)
{
cout<<"\nRooms data is not available.\nPlease add the rooms first.";
getch();
}
else
hm.getAvailRoom();
break;
case 4:
if(count1==0)
{
cout<<"\nRooms are not available.\nPlease add the rooms first.";
getch();
}
else
{
cout<<"Enter Customer Name: ";
cin>>pname;
hm.searchCustomer(pname);
}
break;
case 5:
if(count1==0)
{
cout<<"\nRooms are not available.\nPlease add the rooms first.";
getch();
}
else
{
hm.checkOut();
}
break;
case 6:
hm.guestSummaryReport();
break;
case 7:
cout<<"\nTHANK YOU! FOR USING SOFTWARE";
break;
default:
cout<<"\nPlease Enter correct option";
break;
}
}while(opt!=7);

getch();

    return 0;
}
