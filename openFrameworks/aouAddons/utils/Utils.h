#pragma once
/*
 *  Utils.h
 *  backdrop
 *
 *  Created by Robin Beitra (robin@beitra.net) on 05/03/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include <vector>
#include <string>
#include <map>
#include <list>
#include <set>
#include <typeinfo>
#include "ofMain.h"

using namespace std;

template<class T>
class ProxyValue: public T{
public:
	vector<T*> targets;
	
	void push(T& target){
		targets.push_back(&target);
	};
	
    T& operator = (const T& other){
		int i;
		for(i = targets.size()-1; targets >=0; --i){
			(*targets[i]) = other;
		}
	};
};

template<class T>
class SharedValue{
public:
	T value;
};

template<class T>
class SharedValues{
public:
	map< string, SharedValue<T>* > mapped;
	
	static T& global(string name = ""){
		static SharedValues<T> shared;
		SharedValue<T>* result;
		if(shared.mapped.find(name) == shared.mapped.end()){
			result = new SharedValue<T>();
			shared.mapped[name] = result;
		} else {
			result = shared.mapped[name];//->second;
		}
		return result->value;
		//if(mapped
	};
};

template<class T>
class PropertyValue{
private:
	T _value;
public:
	T value;
	bool modified(){
		return value != _value;
	};
	bool update(){
		bool m = modified();
		value = _value;
		return m;
	};
	bool update(T __value){
		bool m = __value != value;
		value = _value = __value;
		return m;
	}
	T get(){
		return _value;
	};
	void set(T val){
		value = val;
	};
};

class PooledValues{
public:
	static map<string, void*(*)(void)>& getAcquirers(){
		static map<string, void*(*)(void)>* acquirers = new map<string, void*(*)(void)>();
		return *acquirers;
	};
	static map<string, void(*)(void*)>& getReleasers(){
		static map<string, void(*)(void*)>* releasers = new map<string, void(*)(void*)>();
		return *releasers;
	};
	static void registerPool(string name, void*(*acquire)(void), void(*release)(void*)){
		(getAcquirers())[name] = acquire;
		(getReleasers())[name] = release;
	};
	static void release(void* value, string name){
		//string name = typeid(value).name();
		if((getReleasers()).find(name) != (getReleasers()).end()){
			void(*releaser)(void*) = (getReleasers())[name];
			releaser(value);
		}
	};
	static void* acquire(string name){
		if((getAcquirers()).find(name) != (getAcquirers()).end()){
			void*(*acquirer)(void) = (getAcquirers())[name];
			return acquirer();
		}
	};
};

template<class T>
class PooledValue{
public:
	static list<T*>* getPool(){
		static list<T*>* pool = new list<T*>();
		return pool;
	};
	static string getType(){
		return typeid(T).name();
	};
	static void registerPool(){
		PooledValues::registerPool(getType(), (void*(*)(void))(acquire), (void(*)(void*))(release));
	};
	static T* acquire(){
		T* result;
		if(getPool()->size() == 0){
			result = new T();
		} else {
			result = getPool()->back();
			getPool()->pop_back();
		}
		return result;
	};
	static void release(T* value){
		getPool()->push_back(value);
	};
};

class Stringifier{
public:
	static string stringify(int x){
		static char buffer[1024];
		sprintf(buffer, "%d", x);
		string result = "";
		result.append(buffer);
		return result;
	}
	static string stringify(float x){
		static char buffer[1024];
		sprintf(buffer, "%f", x);
		string result = "";
		result.append(buffer);
		return result;
	}
	static string stringify(char* x){
		string result = "";
		result.append(x);
		return result;
	}
};
#define _S(x)	Stringifier::stringify(x)

#define _T(x) cout << x << std::endl;


static int GUID(){
	static int _guid = 0;
	return ++_guid;
};

#include <algorithm>
//From http://www.richelbilderbeek.nl
template <class Container>
void Append(Container& destination, const Container& source){
	std::copy(source.begin(), source.end(), std::back_inserter(destination));
};

template <class Item>
void Shuffle(vector<Item>& items){
	map<int, Item> copy;
	int i = items.size();
	Item item;
	int key;
	while(--i>=0){
		item = items.back();
		items.pop_back();
		while(copy.find(key=rand()) != copy.end()){
		}
		copy[key] = item;
	}
	typename map<int, Item>::iterator it;
	for(it = copy.begin(); it != copy.end(); ++it){
		items.push_back(it->second);
	}
};

template <class Item>
void Shuffle(list<Item>& items){
	map<int, Item> copy;
	int i = items.size();
	Item item;
	int key;
	while(--i>=0){
		item = items.back();
		items.pop_back();
		while(copy.find(key=rand()) != copy.end()){
		}
		copy[key] = item;
	}
	typename map<int, Item>::iterator it;
	for(it = copy.begin(); it != copy.end(); ++it){
		items.push_back(it->second);
	}
};

template <class K, class T>
T& MapFindOrCreate(map<K, T>& collection, K key){	
	if(collection.find(key) == collection.end()){
		T* t = new T();
		collection[key] = *(t);
		delete t;
	}
	return collection[key];
};

template <class K, class T>
T MapEraseIfExists(map<K, T>& collection, K key){
	typename map<K, T>::iterator it = collection.find(key);
	if(it == collection.end()){
		return NULL;
	} else {
		T t = (it->second);// = new T();
		collection.erase(it);
		return t;
	}
};

template <class T>
T ListEraseIfExists(list<T>& collection, T item){
	typename list<T>::iterator it = collection.begin();
	while (it != collection.end()) {
		T t = *it;
		if (t == item) {
			collection.erase(it, it);
			return t;
		}
		++it;
	}
	return NULL;
};

template <class T>
typename list<T>::iterator ListGetIndex(list<T>& collection, int index){
	if(index >= 0 && index < collection.size()){
		typename list<T>::iterator it = collection.begin();
		while(--index>=0){
			++it;
		}
		return it;
	} else {
		return collection.end();
	}
};

template <class T>
typename set<T>::iterator SetGetIndex(set<T>& collection, int index){
	if(index >= 0 && index < collection.size()){
		typename set<T>::iterator it = collection.begin();
		while(--index>=0){
			++it;
		}
		return it;
	} else {
		return collection.end();
	}
};

template <class Item>
void ClearPointerList(list<Item*>& items){
	typename list<Item*>::iterator it;
	for(it = items.begin(); it != items.end(); ++it){
		delete *it;
	}
	items.clear();
};

template <class Item>
void ClearPointerVector(vector<Item*>& items){
	typename vector<Item*>::iterator it;
	for(it = items.begin(); it != items.end(); ++it){
		delete *it;
	}
	items.clear();
};

template <class Item>
void ClearPointerSet(set<Item*>& items){
	typename set<Item*>::iterator it;
	for(it = items.begin(); it != items.end(); ++it){
		delete *it;
	}
	items.clear();
};


template <class K, class Item>
void ClearPointerMap(map<K, Item*>& items){
	typename map<K, Item*>::iterator it;
	for(it = items.begin(); it != items.end(); ++it){
		delete it->second;
	}
	items.clear();
};

template<class K>
bool DeleteIfNotNull(K*& item){
	if(item != NULL){
		delete item;
		item = NULL;
		return true;
	} else {
		return false;
	}
};

template <class T>
typename vector<T>::iterator VectorFindItem(vector<T>& items, T& item){
	typename vector<T>::iterator it;
	for(it = items.begin(); it != items.end(); ++it){
		if(*it == item) break;
	}
	return it;
};

template <class T>
T* VectorRandomItem(vector<T*>& items){
	int max = items.size();
	if(max > 0){
		int r = ofRandom(0, max);
		r = r%max;
		return items[r];
	} else {
		return NULL;
	}
};

//http://stackoverflow.com/questions/236129/c-how-to-split-a-string
template < class ContainerT >
void tokenize(const string& str, ContainerT& tokens, const string& delimiters = " ", const bool trimEmpty = false)
{
	string::size_type pos, lastPos = 0;
	while(true)
	{
		pos = str.find_first_of(delimiters, lastPos);
		bool isEnd = pos == string::npos;
		
		if(isEnd){
			pos = str.length();
		}
		
		if(pos != lastPos || !trimEmpty){
			string::size_type len = pos - lastPos;
			string substr = string(str.data()+lastPos, len);
			tokens.push_back(substr);
		}
		
		if(isEnd){
			break;
		}
		lastPos = pos + 1;
	}
};