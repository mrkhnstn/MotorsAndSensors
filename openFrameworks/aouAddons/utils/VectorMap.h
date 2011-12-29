#pragma once
/*
 *  VectorMap.h
 *  backdrop
 *
 *  Created by Robin Beitra (robin@beitra.net) on 04/03/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

//#include "boost/static_assert.hpp"
#include <vector>
#include <map>
//using namespace std;

template <class K, class V>
class VectorMap{
public:
	std::vector<V> indexed;
	std::map<K, int> mapped;
	std::map<int, K> reverse;
	
	int add(K key, V value){
		int index = -1;
		if(mapped.find(key) == mapped.end()){
			index = indexed.size();
			indexed.push_back(value);
			mapped[key] = index;
			reverse[index] = key;
		}
		return index;
	};
	
	int find(K key){
		//return 0;
		//typename std::map<K, int>::iterator index = mapped.find(key);
		//index = mapped.find(key);
		//if(mapped.find(key) != mapped.end()){
			return mapped.find(key)->second;
		//} else {
		//	return -1;
		//}
	};
	
	K key(int index){
		return reverse[index];
	}
	
	bool hasKey(K key){
		return (mapped.find(key) != mapped.end());
	}
	
	int end(){
		return mapped.end()->second;
	};
	
	int begin(){
		return mapped.begin()->second;
	};
	
	int size(){
		return indexed.size();
	};
	
	void clear(){
		mapped.clear();
		indexed.clear();
		reverse.clear();
	}
	
	
	V operator[](K key){
		//map<K, int>::iterator index;
		//typename map < K, int >::iterator index;
		//I index = mapped.find(key);
		if(mapped.find(key) != mapped.end()){
			return indexed[mapped.find(key)->second];
		} else {
			return NULL;
		}
	};
	
	V operator[](int index){
		return indexed[index];
	};
	
	V& at(int index){
		return indexed.at(index);
	};
	
	V& at(K& key){
		return indexed.at(mapped.find(key)->second);
	};
};