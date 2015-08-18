#pragma once
#include <winsock.h>
#include <windows.h>
#include <queue>
#include <string>

using namespace std;

class ClientSocket
{
private:
	SOCKET client_socket_;
	string server_IP_addr_;
	string server_port_;

	static const int max_buf_size_ = 1024;

	enum mission_type
	{
		MISSION_RECEIVE,
		MISSION_SEND,
	};
	struct socket_mission
	{
		mission_type mission_ID;
		string send_str;												//����IDΪMISSION_SENDʱ��Ч
	};
	queue<socket_mission> mission_queue_;
	volatile bool running_flag_;
	volatile bool stop_flag_;
	char receive_buf_[max_buf_size_];
	HANDLE socket_thread_;

	static bool win_socket_init();
	static DWORD WINAPI on_socket_running(LPVOID data);
	void clean_mission_queue();
protected:
	//�ص�����
	virtual void on_init_failed(){}
	virtual void on_socket_create_failed(int WSA_error_code){}
	virtual void on_connect_failed(SOCKET socket, int WSA_error_code){}
	virtual void on_connect_success(SOCKET socket){}
	virtual void on_receive_failed(SOCKET socket, int WSA_error_code){}
	virtual void on_receive_completed(SOCKET socket, char* str, int length){}
	virtual void on_send_failed(SOCKET socket, int WSA_error_code){}
	void send_stop_message();																						//�ú��������ڻص������з�����ֹ��Ϣ����ֹsocket�ػ��߳�
public:
	ClientSocket();
	virtual ~ClientSocket();

	void set_server_IP_addr(const char* IP_addr)
	{
		server_IP_addr_ = IP_addr;
	}

	void set_server_port(const char* port)
	{
		server_port_ = port;
	}

	void start();																				//����socket�ػ��߳�
	void stop();																				//ֹͣsocket�ػ��̣߳������ڻص�������ʹ�á��ص���������ʹ��send_stop_message()����
	void receive();																			//������Ϣ
	void send_string(const char* str);									//������Ϣ

	bool is_run()
	{
		return running_flag_;
	}
};
