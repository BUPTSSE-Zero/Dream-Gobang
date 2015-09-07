#pragma once
#include "graphics.h"
class Image
{
	private:
		PIMAGE image_;
	public:
		/**
		* @brief ��ָ�����ļ�����Image����
		* @param file_path ָ���ļ���·��
		*/
		Image(const char* file_path);

		/**
		* @brief �ӵ�ǰ�Ĵ���ͼ����Image����
		*/
		Image();

		/**
		* @brief ���ڴ��е�ͼ�����ݹ���Image����
		* @param mem_data �洢ͼ�����ݵ�����
		* @param data_size �������ݵ��ܴ�С
		*/
		Image(const void* mem_data, long data_size);
		~Image();

		/**
		 * @brief ��ͼ����ʾ����Ļ��
		 * @param x ͼ����ʾ��Ŀ����������Ͻ�x����
		 * @param y ͼ����ʾ��Ŀ����������Ͻ�y����
		 */
		void show_image(int x, int y) const;

    /**
		 * @brief ��ͼ����ָ����͸������ʾ����Ļ��
		 * @param x ͼ����ʾ��Ŀ����������Ͻ�x����
		 * @param y ͼ����ʾ��Ŀ����������Ͻ�y����
		 * @param alpha ͸���ȣ���ΧΪ0.0 - 1.0��0.0Ϊ��ȫ͸����1.0Ϊ��ȫ��͸��
		 * @attention ��PNG��ʽ��ͼƬ����ʹ�ô˺���,PNGͼƬ��Ҫ������ʾ��ʹ�ô˺���
		 */
		void show_image_with_alpha(int x, int y, double alpha) const;

		int get_width()
		{
			return getwidth(image_);
		}

		int get_height()
		{
			return getheight(image_);
		}
};
