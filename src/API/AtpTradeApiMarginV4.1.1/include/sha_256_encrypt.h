#ifndef _SHA_ENCRYPTED_PASSWORD_H_
#define _SHA_ENCRYPTED_PASSWORD_H_

#define ENCRYPT_API_USE_STATIC

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
	* @brief 密码加密接口		
	* @param[in] 	buf 加密信息
	* @param[in]  len 加密信息长度
    * @bparam[in] out_len 加密后的数据长度
	* @return 		加密后的信息
	*
	* 注意：要求在接口内申请内存保存加密后的内容作为返回值，由调用者释放内存
	*/
	ENCRYPT_API void* EncryptedPasswordSha256(const char* buf, unsigned int len, unsigned int *out_len,void* ext_buffer,unsigned int ext_buffer_len);

	/**
	* @brief 密码解密接口		
	* @param[in] 	buf 解密信息
	* @param[in]  len 解密信息长度
    * @bparam[in] out_len 解密后的数据长度
	* @return 		解密后的信息
	*
	* 注意：该加密算法属于不可逆算法，所以不支持解密操作直接返回nullptr
	*/
    ENCRYPT_API void *DecryptPasswordSha256(const char *buf, unsigned int len, unsigned int *out_len, void *ext_buffer, unsigned int ext_buffer_len);
}

#endif /*_SHA_ENCRYPTED_PASSWORD_H_*/
