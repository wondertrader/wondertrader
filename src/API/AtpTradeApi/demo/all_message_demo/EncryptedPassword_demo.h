#ifndef _ENCRYPTED_PASSWORD_H_
#define _ENCRYPTED_PASSWORD_H_

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
	void* EncryptedPassword(const char* buf, unsigned int len, unsigned int *out_len);
}



#endif /*_ENCRYPTED_PASSWORD_H_*/
