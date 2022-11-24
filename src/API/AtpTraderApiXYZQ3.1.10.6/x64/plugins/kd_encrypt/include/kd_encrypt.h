#ifndef _RSA_ENCRYPTED_PASSWORD_H_
#define _RSA_ENCRYPTED_PASSWORD_H_

#if defined _WIN32
#   if    defined ENCRYPT_API_USE_STATIC
#      define  ENCRYPT_API  
#   elif  defined ENCRYPT_API_BUILD_EXPORT	
#      define  ENCRYPT_API __declspec(dllexport)
#   else
#      define  ENCRYPT_API __declspec(dllimport)
#   endif
#else 
#      define  ENCRYPT_API  __attribute__((visibility("default")))
#endif

extern "C"
{
	/**
	* @brief ������ܽӿ�		
	* @param[in] 	buf ������Ϣ
	* @param[in]  len ������Ϣ����
    * @bparam[in] out_len ���ܺ�����ݳ���
	* @return 		���ܺ����Ϣ
	*
	* ע�⣺Ҫ���ڽӿ��������ڴ汣����ܺ��������Ϊ����ֵ���ɵ������ͷ��ڴ�.������Ҫ���û�������RSA_PUBLIC_KEY_PATH��ָ��RSA��Կ·��
	*/
	ENCRYPT_API void *EncryptedPassword(const char *buf, unsigned int len, unsigned int *out_len, void *ext_buffer, unsigned int ext_buffer_len);

	/**
	* @brief ������ܽӿ�		
	* @param[in] 	buf ������Ϣ
	* @param[in]  len ������Ϣ����
    * @bparam[in] out_len ���ܺ�����ݳ���
	* @return 		���ܺ����Ϣ
	*
	* ע�⣺Ҫ���ڽӿ��������ڴ汣����ܺ��������Ϊ����ֵ���ɵ������ͷ��ڴ�.������Ҫ���û�������RSA_PRIVATE_KEY_PATH��ָ��RSA˽Կ·��
	*/
	ENCRYPT_API void *DecryptPassword(const char *buf, unsigned int len, unsigned int *out_len, void *ext_buffer, unsigned int ext_buffer_len);
}

#endif /*_RSA_ENCRYPTED_PASSWORD_H_*/
