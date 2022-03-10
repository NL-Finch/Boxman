#include <stdio.h>
#include <mysql.h> 
int main(void)
{
	MYSQL mysql;  // ���ݿ���
	MYSQL_RES* res;  // ��ѯ�����
	MYSQL_ROW row;  // ��¼�ṹ��

	// ��ʼ�����ݿ�
	mysql_init(&mysql);

	// �����ַ�����
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	// �������ݿ�
	if (mysql_real_connect(&mysql, "127.0.0.1", "root", "Acqc315271",
		"school", 3306, NULL, 0) == NULL)
	{
		printf("����ԭ��%s \n", mysql_error(&mysql));
		printf("����ʧ�ܣ�\n");
		exit(-1);  // �˳�����
	}

	// ��ѯ����
	int ret = mysql_query(&mysql, "select * from student;");

	// ��ȡ�����
	res = mysql_store_result(&mysql);

	// ��ROW��ֵ���ж�ROW�Ƿ�Ϊ�գ���Ϊ�վʹ�ӡ����
	while (row = mysql_fetch_row(res))
	{
		printf("%s  ", row[0]);   // ��ӡID
		printf("%s  ", row[1]);   // ��ӡ����
		printf("%s  ", row[2]);   // ��ӡ�༶
		printf("%s  \n", row[3]); // ��ӡ�Ա�
	}

	// �ͷŽ����
	mysql_free_result(res);

	// �ر����ݿ�
	mysql_close(&mysql);

	system("pause");
	return 0;
}







//----------------------------------------------------------------------------------------

//#include"mysql.h"
//
//int main()
//{
//	DataBase* d1 = new DataBase;
//	//����
//	d1->Connect("localhost", "root", "Acqc315271", "test", 0);
//	printf("\n");
//	//��ѯ��
//	d1->Query("t_student");
//	printf("\n");
//	//�������
//	d1->Implement("insert into t_student values(201916010001, '����', '2019-09-09', now())");
//	printf("\n");
//	//��ѯ��
//	d1->Query("t_student");
//	printf("\n");
//	return 0;
//}


