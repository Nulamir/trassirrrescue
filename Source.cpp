
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <windows.h>
#include <time.h> 

#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

#include <stdio.h>
#include "sqlite3.h"
#include "sqlite3.c"

#define __CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

//namespace fs = std::experimental::filesystem;

using namespace std;

#define _CRT_SECURE_NO_WARNINGS

#ifdef _MSC_VER
#define fwrite_unlocked _fwrite_nolock
#endif


//����� �����
string camers_name[14];
//���� �����
string camers_code[14];
//������� ������ (��� ����� �����)
string	current_camera_name = "none";
//����� ���� ������� (��� ����� �����)
string	current_datetime_stamp = "nether";

string	current_date_dir = "nether";
string	current_hour_dir = "nether";

//������������ ������� ����� ��� ������ �������� ������
unsigned int max_cadre_size = 5000000;

//��� �������� �����
//unsigned char peer0_0[] = { 0xc1, 0x02, 0x06, 0x08, 0x00, 0x5e, 0x84 };
unsigned char i_code[] = { 0x68,0x32,0x36,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x49 };

//��� ����������� �����
unsigned char p_code[] = { 0x68,0x32,0x36,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70 };
//��� ����������� �����
unsigned char p2_code[] = { 0x68,0x32,0x36,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x50 };

//������� �����
BYTE* large_buffer = NULL;



time_t now ;
// �������� �������� �����
time_t t_gauge1;
time_t t_gauge2;
struct tm ltm;

const char* SQL = "CREATE TABLE IF NOT EXISTS foo(a,b,c); INSERT INTO FOO VALUES(1,2,3); INSERT INTO FOO SELECT * FROM FOO;";


bool find_camera(__int64 i)
{

	//���������� ��� ������
	for (int j = 0; j < 14; j++)
	{
		//������� ��� �������

		for (int k = 0; k < 8; k++)
		{
			if (camers_code[j][k] != large_buffer[i + k])
			{
				break;
			}
			if (k == 7)
			{
				current_camera_name = camers_name[j];
				return true;
			}
		}
	}
	return false;
}


bool find_i(__int64 i)
{


		//������� ��� �������

	for (int k = 0; k < sizeof(i_code); k++)
	{
		if (i_code[k] != large_buffer[i + k])
		{
			break;
		}
		if (k == sizeof(i_code) - 1)
		{
			return true;
		}
	}
	return false;
}

bool find_p(__int64 i)
{


	//������� ��� �������

	for (int k = 0; k < sizeof(p_code); k++)
	{
		if (!(p_code[k] == large_buffer[i + k] || p2_code[k] == large_buffer[i + k]))
		{
			break;
		}
		if (k == sizeof(p_code) - 1)
		{
			return true;
		}
	}
	return false;
}


bool find_datetime_stamp(__int64 i)
{
	unsigned int year;
	std::string s_year;
	std::stringstream out_year;
	unsigned int month;
	std::string s_month;
	std::stringstream out_month;
	unsigned int day;
	std::string s_day;
	std::stringstream out_day;

	unsigned int hour;
	std::string s_hour;
	std::stringstream out_hour;
	unsigned int minute;
	std::string s_minute;
	std::stringstream out_minute;
	unsigned int second;
	std::string s_second;
	std::stringstream out_second;

	unsigned int msecond;
	std::string s_msecond;
	std::stringstream out_msecond;
	//unsigned int year;

	year = large_buffer[i - 16] + large_buffer[i - 15]*256;
	out_year << year;
	s_year = out_year.str();

	month = large_buffer[i - 14];
	out_month << std::setw(2) << std::setfill('0') << month;
	s_month = out_month.str();

	day = large_buffer[i - 13];
	out_day << std::setw(2) << std::setfill('0') << day;
	s_day = out_day.str();

	hour = large_buffer[i - 12];
	out_hour << std::setw(2) << std::setfill('0') << hour;
	s_hour = out_hour.str();

	minute = large_buffer[i - 11];
	out_minute << std::setw(2) << std::setfill('0') << minute;
	s_minute = out_minute.str();

	second = large_buffer[i - 10];
	out_second << std::setw(2) << std::setfill('0') << second;
	s_second = out_second.str();

	msecond = large_buffer[i - 21];
	out_msecond << std::setw(2) << std::setfill('0') << msecond;
	s_msecond = out_msecond.str();


	current_datetime_stamp = s_hour + "-" + s_minute + "-" + s_second + " " + s_day + "-" + s_month + "-" + s_year;
	current_date_dir = s_day + "-" + s_month + "-" + s_year;
	current_hour_dir = s_hour;
	//std::cout << std::setw(2) << std::setfill('0') << 1;
	return false;
}



string datetime_stamp()
{
	std::string ss;
	std::stringstream s_out;

	now = time(0);
	localtime_s(&ltm ,&now );

	// print various components of tm structure.
	//cout << "Year" << 1970 + ltm->tm_year << endl;
	//cout << "Month: " << 1 + ltm->tm_mon << endl;
	//cout << "Day: " << ltm->tm_mday << endl;
	//cout << "Time: " << 1 + ltm->tm_hour << ":";
	//cout << 1 + ltm->tm_min << ":";
	//cout << 1 + ltm->tm_sec << endl;


	s_out << 1900 + ltm.tm_year << "-" << 1 + ltm.tm_mon << "-" << ltm.tm_mday << " " << ltm.tm_hour << "-" << ltm.tm_min << "-" << ltm.tm_sec;
	ss = s_out.str();

	return ss;
}



int main()
{
	// ���������� ��������� ���������� (������� �����).
	HANDLE partition = INVALID_HANDLE_VALUE;
	// �������� � �������.
	PARTITION_INFORMATION partitionInfo = { 0 };
	// �������� � ��������� �����, �� ������� ���������� ������.
	DISK_GEOMETRY diskGeometry = { 0 };

	// ���������� ����� ��� ���������� ������ �������.
	HANDLE file = INVALID_HANDLE_VALUE;

	// ����� ��� ������.
	BYTE* buffer = NULL;
	// ������ ������.
	DWORD bufferSize = 0;

	// ���������� ������������ ����.
	DWORD bytesReturned = 0;
	// ���������� ���������� ����.
	DWORD bytesWritten = 0;

	// ��������� ����������.
	BOOL result; BOOL result1;

	//���� ������ �� ��������� ������
	bool camera_found = false;
	bool need_to_open_file = false;

	__int64 sectors;




	// ��������� ������ ����� ������.
	partition = CreateFile("\\\\.\\I:",
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);


	if (partition == INVALID_HANDLE_VALUE)
	{
		cout << "read disk (H) dont open" << endl;
		cout << "Error: " << GetLastError() << endl;
	}

	// �������� ����� ��� ����������  �����.

	std::ofstream out;          // ����� ��� ������
	std::ofstream out1;          // ����� ��� ������
	std::ofstream out2;          // ����� ��� ������

	string logfile;
	string outfile;
	string s_outdatedir;
	string s_outhourdir;

	
	logfile = "U:\\" + datetime_stamp() + ".log";
	
	
	out2.open(logfile, ios::binary); // �������� ���� ��� ������

	
	if (!out.is_open())
	{
		cout << "write disk (F) dont open" << endl;
		cout << "Error: " << GetLastError() << endl;
	}

	// ����������� �������� � ��������� �����, �� ������� ���������� ������.
	if (!DeviceIoControl(partition,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL,
		0,
		&diskGeometry,
		sizeof(DISK_GEOMETRY),
		&bytesReturned,
		(LPOVERLAPPED)NULL))
	{
		cout << "Error: " << GetLastError() << endl;
		CloseHandle(partition);
	}

	// ������ �������.
	//cout << "Partition's size (in bytes): " << partitionInfo.PartitionLength.QuadPart << endl;
	// ������ ����� ���������� �� ������� ������� (������, ����� ����������� ����������).
	cout << "Disk's size (in bytes):      " << (diskGeometry.Cylinders.QuadPart) *
		diskGeometry.TracksPerCylinder * diskGeometry.SectorsPerTrack *
		diskGeometry.BytesPerSector << endl;
	cout << "Num of cylinders:    " << diskGeometry.Cylinders.QuadPart << endl;
	cout << "Tracks per cylinder: " << diskGeometry.TracksPerCylinder << endl;
	cout << "Sectors per track:   " << diskGeometry.SectorsPerTrack << endl;
	cout << "Bytes per sector:    " << diskGeometry.BytesPerSector << endl;
	//system("PAUSE");


	// ������������� ������ ������.
	bufferSize = diskGeometry.BytesPerSector * diskGeometry.SectorsPerTrack;
	
	//������������ ������ ����� � ��������
	__int64 max_sector = (diskGeometry.Cylinders.QuadPart)*diskGeometry.TracksPerCylinder* diskGeometry.SectorsPerTrack; 
	//�� ��� ������ ����� � � ��� ��������, � ��� �� �������� �� ���������� ����� ������ �� ���� ���� ��� ��� ����4
	max_sector = 7814014976;
	
	//��������� ������

	//���������� ������������ ��� ����������� �������� �����������.
	//int sector_size = 512;
	__int64 from_sector = 6448250880;    //��������� ������
	//21 ������� ����� �����
	from_sector = 0.25 * max_sector;
	//from_sector = 6448250915;
	//from_sector = 4448328494;
	//from_sector = 7813644216;1869661822
	from_sector = 1772257280;
	from_sector = 2148799741;
	//1772257280 ������ �� �� �������� �����
	unsigned long scoup_size = 100000000;	//������ ����������� �������
	//scoup_size = 10000000;	//������ ����������� �������
	//double double_buffersize = bufferSize;
	//int_buffersize = bufferSize;

	//������������ ������ ����� ���������
	unsigned long max_bytes_on_u = 1900000000 ;

	float delenie = (double) scoup_size / bufferSize;
	//������ ����������� ������� � �������
	unsigned long scoup_buffer_size = ceil(delenie);
	//���������� ��� ���������� ������


	unsigned long scoup_sector_size = scoup_buffer_size * diskGeometry.SectorsPerTrack;
	//���������� ��� ���������� ������

	 delenie = (double)3000000 / diskGeometry.BytesPerSector;
	 unsigned long  scoup_overlap = ceil(delenie);	//
	//delenie = 0.56;
	cout << "scoup_sector_size:    " << scoup_sector_size << endl;

	__int64 scoup_max_size = bufferSize * scoup_buffer_size; //������ ����������� ������� � ����������� � ������� �������


	camers_name[0] = "DS - 2CD2622FWD - IS_1";
	camers_code[0] = "pOUKpl96";
	camers_name[1] = "DS - 2CD2T55FWD - I5_1";
	camers_code[1] = "GLSDMzDm";
	camers_name[2] = "DS - 2CD2523G0 - IS_1";
	camers_code[2] = "xxa15omi";
	camers_name[3] = "DS - 2CD2523G0 - IS_2";
	camers_code[3] = "HX9CxQMz";
	camers_name[4] = "DS - 2CD2523G0 - IS_3";
	camers_code[4] = "xLviweM6";
	camers_name[5] = "DS - 2CD2523G0 - IS_4";
	camers_code[5] = "g68Kfmvu";
	camers_name[6] = "DS - 2CD2643G0 - IZS_1";
	camers_code[6] = "TzfTx8os";
	camers_name[7] = "DS - 2CD2643G0 - IZS_2";
	camers_code[7] = "LKpfLS6y";
	camers_name[8] = "DS - 2CD2643G0 - IZS_3";
	camers_code[8] = "yZVksUjO";
	camers_name[9] = "DS - 2CD2822F(B)20170822AAWR_1";
	camers_code[9] = "aVfW98lt";
	camers_name[10] = "DS - 2CD2822F(B)20170822AAWR_2"; //0
	camers_code[10] = "M0TqsWQF";
	camers_name[11] = "DS - 2CD2822F(B)20170822AAWR_3";
	camers_code[11] = "cuVGjH0S";
	camers_name[12] = "DS - 2CD2822F(B)20170822AAWR_4";
	camers_code[12] = "vxfqcGE2";
	camers_name[13] = "DS - 2CD2822F(B)20170822AAWR_5";
	camers_code[13] = "KVkHcRYJ";

	int i_shift = 36; // ��������� ������� �������� ����� ����� ��������� ���� ������
	int c_num_bytes = 15; //����� ����� �������� �����



	unsigned long s; //������� ��������� ����
	unsigned long cadre_size;

	bool disk_space_on_u_off;
	bool max_sector_on_i_exceed = false;
	unsigned long n = 0;
	disk_space_on_u_off = false;

	bool i_found = false;
	bool p_found = false;
	unsigned int	i_size = 100000;
	unsigned int	p_size = 1000;

	//std::thread::hardware_concurrency();


	sqlite3 * db = 0; // ����� ������� ���������� � ��
	char* err = 0;

	// ��������� ����������
	if (sqlite3_open("my_cosy_database.dblite", &db))
		fprintf(stderr, "������ ��������/�������� ��: %s\n", sqlite3_errmsg(db));
	// ��������� SQL
	else if (sqlite3_exec(db, SQL, 0, 0, &err))
	{
		fprintf(stderr, "������ SQL: %sn", err);
		sqlite3_free(err);
	}
	// ��������� ����������
	sqlite3_close(db);










	do
	{
		s = 0;	
		t_gauge1 = time(0);

		//����� ���� ���������� �������
		large_buffer = new BYTE[scoup_max_size];
		// ��������� ������ ��� ������ ���������� �������.
		buffer = new BYTE[bufferSize];

		stringstream sn;
		sn << n;
		string strn = sn.str();

		outfile = "U:\\out-" + strn;
		outfile = outfile + ".hex";

		out2 << "[" << datetime_stamp() << "]" <<"step: " << outfile <<" from sector: " << from_sector << " s: " << s << endl;

		LARGE_INTEGER qPosition;
		qPosition.QuadPart = from_sector * diskGeometry.BytesPerSector; //!!!!!!!!!!!!! 128
		//������������ �� ������ ������
		SetFilePointer(partition, qPosition.LowPart, &qPosition.HighPart, FILE_BEGIN);

		// ����������� ������� � ����.

		do
		{
			// ������ �������� �������.
			result = ReadFile(partition, &large_buffer[s], bufferSize, &bytesReturned, NULL);
			if (!result)
			{
				cout << "Error: " << GetLastError() << endl;
				delete[] buffer;
				CloseHandle(file);
				CloseHandle(partition);
				return (-7);
			}

			//for (int i = 0; i < bufferSize; i++)
			//{
			//	large_buffer[i + s] = buffer[i];

			//}
			s += bufferSize;
			max_bytes_on_u -= bufferSize;

			if (max_bytes_on_u <= 0)
			{
				disk_space_on_u_off = true;
				out2 << "[" << datetime_stamp() << "]" << "max disk size on u exceeded, max sector: " << from_sector + scoup_sector_size << endl;

			}

		} while (result && bytesReturned && (s < scoup_max_size) && !disk_space_on_u_off);


		//out.open(outfile, ios::binary); // �������� ���� ��� ������
		//s = 0;
		//do
		//{

		//	out << large_buffer[s];
		//	s += 1;

		//} while (result && bytesReturned && (s < scoup_max_size));

		n++;
		
		//out2 << "[" << datetime_stamp() << "]" << " buffer out size: " << out.tellp() << endl;

		//out.close(); // ��������� ����
		//out.flush();

		//������� �� ������� ���� �������
		//BYTE* current_frame;
		//BYTE* current_string;


		//���������� ��� �������
		for (unsigned long i = 0; i < scoup_max_size; i++) //=0
		{
			//���� ������
			if (find_camera(i))
			{
				need_to_open_file = true; //����� �� ��� �� ����� ��������� �� ������� ���� ��� ��� ���������� � �������
				out2 << "[" << datetime_stamp() << "]" << "camera code:  " << current_camera_name << "camera dt:  " << current_datetime_stamp <<  " begin of camera code:    " << i << endl;
				//������������ � ���������� ������������ �����
				i += i_shift;
				if (find_i(i))
				{
					out2 << "[" << datetime_stamp() << "]" << "begin of i frame:    " << i << endl;
					//������� ������� ������
					if (need_to_open_file)
					{
						find_datetime_stamp(i);

						s_outdatedir = "U:\\" + current_date_dir;
						CreateDirectory(TEXT(s_outdatedir.c_str()), NULL);
						s_outhourdir = "U:\\" + current_date_dir + "/" + current_hour_dir;
						CreateDirectory(TEXT(s_outhourdir.c_str()), NULL);
						s_outhourdir = "U:\\" + current_date_dir + "/" + current_hour_dir + "/" + current_camera_name;
						CreateDirectory(TEXT(s_outhourdir.c_str()), NULL);


					
						//out1.open("U:\\" + current_date_dir + "/" + current_hour_dir + "/" + current_camera_name + "/"  + current_datetime_stamp + " "+ current_camera_name+ ".h264", ios::binary); // �������� ���� ��� ������
						need_to_open_file = false;
					}

					cadre_size = 0;

					//������������ � ������
					i += sizeof(i_code) + c_num_bytes;

					camera_found = false;



					//i_found = true;
					do
					{

						//��������� 
						//out1 << large_buffer[i];

						//������ �� ����. �������				   
						i++;
						cadre_size++;

						//������� ��������� ���������
						//if (i_found)
						//{
						//	for (unsigned int j = 0; j < i_size; j++)
						//	{
						//		out1 << large_buffer[i];
						//		//������ �� ����. �������				   
						//		i++;
						//		cadre_size++;
						//		i_found = false;
						//	}
						//}
						//if (p_found)
						////{
							//for (unsigned int j = 0; j < p_size; j++)
							//{
					//			out1 << large_buffer[i];
						//		//������ �� ����. �������				   
							//	i++;
							//	cadre_size++;
						//		p_found = false;
						//	}
						///}

						//����� p ���� - ����������
						if (find_p(i))
						{
							out2 << "[" << datetime_stamp() << "]" << "begin of p frame:    " << i << endl;
							out2 << "[" << datetime_stamp() << "]" << "cadre_size:    " << cadre_size << endl;
							i += sizeof(p_code) + c_num_bytes;
							p_found = true;
							cadre_size = 0;

						}

						//����� I ���� - ����������
						if (find_i(i))
						{
							// �������� �� ������ �� ��� ����� ������
							if (find_camera(i - i_shift))
							{
								//out1.close(); // ��������� ����
								//break;
								camera_found = true;
								out2 << "[" << datetime_stamp() << "]" << "begin of new camera:   " << i << endl;
								i = i - i_shift - 1; //i++ � ����� �������� �������

							}
							else
							{
								out2 << "[" << datetime_stamp() << "]" << "begin of i frame:    " << i << endl;
								out2 << "[" << datetime_stamp() << "]" << "cadre_size:    " << cadre_size << endl;
								//�������� � ���� �� ������
								i += sizeof(i_code) + c_num_bytes;
								i_found = true;
								cadre_size = 0;
							}


						}

						//���� ���� ������� ���� ���������� ����, 
						// �������� �� ������� ������� �����, ��� ������ ������ ��� TRASSIR



					//����� �� ������ ������� , �����  �� �����
					} while (!camera_found && i < scoup_max_size && cadre_size < max_cadre_size);
					out2 << "[" << datetime_stamp() << "]" << " file out size: " << out1.tellp() << endl;
					//out1.close(); // ��������� ����
					//out1.flush();
					
				}
				else
				{
					out2 << "[" << datetime_stamp() << "]" << "unseccesseful i cadr of new camera:   " << i << endl;
				}

				//���� �� ������� ������� ����

			}

		}

		from_sector = from_sector + scoup_sector_size - scoup_overlap;

		if (from_sector + scoup_sector_size > max_sector)
		{
			max_sector_on_i_exceed = true;
			out2 << "[" << datetime_stamp() << "]" << "max sector size on I exceeded, max sector: " << from_sector << "scoup_sector_size:    " << scoup_sector_size << endl;

		}

		delete[] buffer;
		delete[] large_buffer;
		delenie = 100.0*((double) from_sector / (double) max_sector);
		system("cls");

		cout << "% :    " << delenie<<" "<< from_sector << "/" << max_sector << endl;


		t_gauge2 = time(0);

		cout << "took:" << t_gauge2 - t_gauge1 << endl;

	}while (!disk_space_on_u_off && !max_sector_on_i_exceed);
	
	
	CloseHandle(file);
	CloseHandle(partition);

	out2.close(); // ��������� ����


	_CrtDumpMemoryLeaks(); 
	return 0;
}


//��������� ������� �������� � �������

// ���� ���� ��������� �������� ����� �� ����������, ���� ���, �� ������������ �� �� ����� � ����� ����

//����� ���������� ���� ������
//���� ��� �������� ����� �� ��������
//������������ � ������������ �� 130 ��
//���� �264, ��������� �� �������� ��� ����� �������� 11���� ���������� ��� �����

//���� � ������������ � �����10�� ���� � ������������ � �����130 ��.� ��������� ����� �� ������ ��� ��, ���� ��, �� ������� � ����� ��������.������������� �����
//�����