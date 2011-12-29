#pragma once
/*
 *  SharedMemory.h
 *  backdrop
 *
 *  Created by Robin Beitra (robin@beitra.net) on 08/02/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

//IMPORTANT: this object has only been tested on OS X and xcode
// int xcode release mode you will have to change the GCC 4.2 Optimization Level to Faster[-02] (other wise it will crash)

#include <sys/mman.h>
//#include <semaphore.h>
#include "ofMain.h"
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <sys/stat.h>

#define read_lock(fd, offset, whence, len)    \
lock_reg  (fd, F_SETLK,  F_RDLCK, offset, whence, len)
#define readw_lock(fd, offset, whence, len)   \
lock_reg  (fd, F_SETLKW, F_RDLCK, offset, whence, len)

#define write_lock(fd, offset, whence, len)   \
lock_reg  (fd, F_SETLK,  F_WRLCK, offset, whence, len)
#define writew_lock(fd, offset, whence, len)  \
lock_reg  (fd, F_SETLKW, F_WRLCK, offset, whence, len)

#define un_lock(fd, offset, whence, len)      \
lock_reg  (fd, F_SETLK,  F_UNLCK, offset, whence, len)

#define    is_readlock(fd, offset, whence, len) \
lock_test(fd, F_RDLCK, offset, whence, len)
#define    is_writelock(fd, offset, whence, len) \
lock_test(fd, F_WRLCK, offset, whence, len)


template <class T>
class ofxSharedObject{
public:

	ofxSharedObject();
	~ofxSharedObject();
	
	T& getObject(); //< access the object of class T in the shared memory
	
	void attach(string _path, bool _init); //< set _init to true ONLY for first process instance 
	//TODO: init is not being used at the moment!!!
	
	void attachLocal(); //< just in case you don't work to with shared memory 
	void detach(); //< being called in destructor, so no need to call explicitly
	
	
	void lockRead(); //< seems to work without using this
	void lockWrite(); //< seems to work without using this
	void unlock();	//< seems to work without using this
	
protected:
	
	T* data;
	
	bool attached;
	char* stringToChars(string* s);
	
	int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);
	pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len);
	
	int size;
	string path;

	int file;
	
};

template <class T>
T& ofxSharedObject<T>::getObject(){
	return *data;
}

template <class T>
char* ofxSharedObject<T>::stringToChars(string* s){
	char* result = new char [s->size()+1];
	strcpy(result, s->c_str());
}

template <class T>
ofxSharedObject<T>::ofxSharedObject(){
	attached = false;
	file = 0;
}

template <class T>
void ofxSharedObject<T>::detach(){
	if(attached){
		if(data>0){
			if(file>0){
				munmap(data, size);
			} else {
				delete data;
			}
		}
		if(file>0){
			unlock();
			close(file);
		}
		attached = false;
	}
}

template <class T>
void ofxSharedObject<T>::attach(string _path, bool _init){
	size = sizeof(T);
	detach();
	path = _path;
	//size = _size;	
	char* str;	
	int err;
	int filemode = O_CREAT | O_RDWR;
	str = stringToChars(&path);
	file = open(str, filemode, 0666);
	err = errno;
	delete[] str;
	if(file>0){
		cout << "fopen success:\t" << _path << ":\t" << file << std::endl;
		//success
		//struct stat buf;
		//fstat(file, &buf);
		//int size = buf.st_size;
		//if(_init){
			void* blank;
			blank = malloc(size);
			write(file, blank, size); //init with nonce sense
			free(blank);
		//}
		int rwmode = PROT_READ | PROT_WRITE;
		data = (T*)mmap(0, size, rwmode, MAP_FILE | MAP_SHARED, file, 0);
		err = errno;
		if(data>0){
			//success
			cout << "mmap success:\t" << _path <<":\t" << data << std::endl;
		} else {
			cout << "mmap failed:\t" << _path << ":\t" << err << std::endl;
		}
	} else {
		cout << "fopen failed:\t" << _path << ":\t" << err << std::endl;
	}
	attached = true;
}


template <class T>
void ofxSharedObject<T>::attachLocal(){	
	data = new T();
	file = 0;
	attached = true;
}

template <class T>
ofxSharedObject<T>::~ofxSharedObject(){	
	detach();
}

template <class T>
void ofxSharedObject<T>::lockRead(){
	if(file>0){
		flock(file, LOCK_SH);
		//readw_lock(file, 0, SEEK_SET, size);
	}
}
template <class T>
void ofxSharedObject<T>::lockWrite(){
	if(file>0){
		flock(file, LOCK_EX);
		//writew_lock(file, 0, SEEK_SET, size);
	}
}
template <class T>
void ofxSharedObject<T>::unlock(){
	if(file>0){
		flock(file, LOCK_UN);
		//un_lock(file, 0, SEEK_SET, size);
	}
}

//template <class T>
//pid_t ofxSharedObject<T>::lock_test(int, int , off_t , int , off_t );

template <class T>
int ofxSharedObject<T>::lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;
	
    lock.l_type   = type;     /* F_RDLCK, F_WRLCK, F_UNLCK         */
    lock.l_start  = offset;   /* byte offset, relative to l_whence */
    lock.l_whence = whence;   /* SEEK_SET, SEEK_CUR, SEEK_END      */
    lock.l_len    = len;      /* #bytes (0 means to EOF)           */
	
    return ( fcntl(fd, cmd, &lock) );
}

template <class T>
pid_t ofxSharedObject<T>::lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;
	lock.l_type = type;     /* F_RDLCK or F_WRLCK */
	lock.l_start = offset;  /* byte offset relative to l_whence */
	lock.l_whence = whence; /* SEEK_SET, SEEK_CUR, SEEK_END */
	lock.l_len = len;       /* #bytes (0 means to EOF) */
	if (fcntl(fd,F_GETLK,&lock) < 0){
		perror("fcntl"); exit(1);}
	if (lock.l_type == F_UNLCK)
		return (0);        /* false, region is not locked by another process */
	return (lock.l_pid); /* true, return pid of lock owner */
}