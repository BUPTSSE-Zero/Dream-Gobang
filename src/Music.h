#pragma once
#define SHOW_CONSOLE
#include "graphics.h"
#include <windef.h>
#include <windows.h>
class Music
{
private:
	static HWND win32_handle_;
	int n_play_;
	int interval_time_;
	HANDLE thread_handle_;
	bool is_running_;
	const char* file_path_;
	float volume_;
	bool pause_mission_;
	bool stop_mission_;
	bool resume_mission_;

	static DWORD WINAPI on_player_running(LPVOID music_player);
public:
  /**
  * @brief ��ָ���������ļ�����һ��Music����
  * @param file_path ָ�����ļ�·����
  * @param n_play ���Ŵ�����-1Ϊ���޴β��š�
  * @param interval_time һ�β����굽��һ�β��ſ�ʼ�ļ��ʱ�䣬��λΪ�롣
  * @note ���������ļ�ʹ��wma��ʽ��MP3��ʽ��ͷ�б�����wav��ʽ�޷���������
  * @attention �����ͷŵ�ʱ�򣬻��Զ�ֹͣ���ֲ��Ų���ֹ����̡߳�
  */
	Music(const char* file_path, int n_play, int interval_time = 0);
	~Music();

  /**
  * @brief ���ü��ʱ�䡣
  * @param time_second ���ʱ�䣬��λΪ�롣
  */
	void set_interval_time(int time_second);

	/**
	* @brief ��������������
	* @param volume ��ǰϵͳ�����ٷֱȣ���ΧΪ0.0 - 1.0��
	* @attention ���õ�������Ҫ����һ�β��ſ�ʼʱ�Ż���Ч��
	*/
	void set_volume(float volume);

	/**
	* @brief �������ֲ��ţ�����������̡߳�
	*/
	void start();

	/**
	* @brief ֹͣ���ֲ��ţ�����ֹ����̡߳�
	* @attention ֹͣ�󣬸ö�����Ѳ��Ŵ���������Ϊ0��
	*/
	void stop();

	/**
	* @brief ��ͣ���ֲ��ţ����������̡߳�
	*/
	void pause();

	/**
	* @brief ����ͣ״̬�ָ����ֲ��ţ����ָ�����̡߳�
	*/
	void resume();
};

