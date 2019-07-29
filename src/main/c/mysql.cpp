#include <iostream>
#include <cstdio>
#include <cstdlib>

// For MySQL Connection
#include <mysql.h>

using namespace std;

// Defining Constant Variables
#define SERVER "localhost"
#define USER "root"
#define PASSWORD "securifi@123"
#define DATABASE "venky"

int main()
{
    MYSQL *connect;
    connect = mysql_init(NULL);

    if (!connect)
    {
        cout << "Mysql Initialization Failed";
        return 1;
    }

    connect = mysql_real_connect(connect, SERVER, USER, PASSWORD,DATABASE,0,NULL,0);

    if (connect)
    {
        cout << "Connection Succeeded\n";
    }
    else
    {
        cout << "Connection Failed\n";
    }

    MYSQL_RES *res_set;
    MYSQL_ROW row;

    // Replace MySQL query with your query

    mysql_query (connect,"DELETE FROM `chat` WHERE `id`=6");

     int i=0;

    res_set=mysql_store_result(connect);

   int numrows = mysql_num_fields(res_set);
    
    cout << " show tables" << DATABASE << " database " << endl;

    while (((row=mysql_fetch_row(res_set))!=NULL))
   {
   for (i = 0; i < numrows; i++)
   {
     cout << row[i] << endl;
   }
   mysql_free_result(res_set);
   printf("\n");
 }
    mysql_close (connect);

    return 0;
}
