#include "convertSourceHex.h"
uint8_t numByte(uint8_t hex){
	return 1;
}
bool checkFile(){
	return true;
}
uint32_t getAddress(){
	return 1;
}
char* getHexArray(){
	char* orginHex = new char[strlen(fileHEX)];
	char* newHex = nullptr;
	char* tempHex = nullptr;
	strcpy(orginHex,fileHex);
	char* firstCheck = nullptr;
	bool check = false;
	while((firstCheck  = strchr(orginHex, ':'))!= nullptr){
		char* secondCheck = strchr(firstCheck + 1, ':');
		uint8_t len = 0;
		if(secondCheck != nullptr)
			 len = firstCheck - secondCheck;
		else
			len = strlen(orginHex) -1;
		if(newHex != nullptr)
			delete[] newHex;
		if(tempHex != nullptr){
			newHex = new char[strlen(tempHex) + len];
			strcpy(newHex,tempHex);
			strncat(newHex,firstCheck + 1,len);
			delete[] tempHex;
		}else{
			newHex = new char[len];
			strncpy(newHex,orginHex + 1,len);
		}
		tempHex = new char[strlen(newHex)];
		strcpy(tempHex,newHex);
		char* reloadHex = new char[strlen(orginHex) - len];
		strcpy(reloadHex, orginHex + len);
		delete[] orginHex;
		orginHex = new char[strlen(reloadHex)];
		strcpy(orginHex, reloadHex);
		delete[] reloadHex;
	}
	 delete[] orginHex;
    delete[] tempHex;
    return newHex;
}
void writeFlash(){
	return;
}