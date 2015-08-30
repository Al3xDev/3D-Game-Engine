#ifndef UTIL_H_H
#define UTIL_H_H


void ptrcpy(void *dest, void *source, uint32_t size)
{
    char *_dest,*_source;
    _dest=(char*)dest;
    _source=(char*)source;
	for(uint32_t i=0;i<size;++i)
	{
		*(_dest+i)=*(_source+i);
	}
}
inline bool FileExist(std::string file_name)
{
    std::ifstream file(file_name.c_str());
    return !file.fail();
}


#endif ///UTIL_H_H
