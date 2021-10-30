#include <iostream>
#include<fstream>
#include<sstream>
#include<dirent.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
extern "C" FILE *popen(const char *command, const char *mode);
string path = ".";
void error(const char *msg){
    perror(msg);
}
string exec(string command) {
   char buffer[128];
   string result;
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
     exit(0);
   }
   while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }
   return result;
}
int connectClient(socklen_t clilen, struct sockaddr_in cli_addr, int sock){
	int con;
	listen(sock,5);
    	clilen = sizeof(cli_addr);
     	con = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
     	if (con < 0)
          error("Connection Error..");
	return con;
}

string parseRequest(char buffer[]){
    string file;
    int i=5,j=5;
    if(buffer[i]==' '){
	file=buffer[i];
	return file;
	}
    while(buffer[i]!= '/'){
        j++;
        i++;
    }
    while(buffer[i--]!= ' ')
        j--;
    i=5;
    while(i<j){
        file+=buffer[i];
        i++;
}
     return file;
}
void fileRead(string filename, string* output){
     string tempfile;
     ifstream RFile(filename);
     while(getline(RFile,tempfile))
    	*(output)+=tempfile + "\n";
}
bool dirExists(const string& dirName_in){
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;
  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;
  return false;
}

int main(int argc, char *argv[]){
         WSADATA wsa;
         if(WSAStartup(MAKEWORD(2,2),&wsa)!=0){
            cout<<"Failed";
            return 1;
         }
	     SOCKET sockfd, confd;
	     istringstream cmd;
         int portno;
	     int clilen;
	     bool dir;
	     char buffer[1024];
	     string listfile, res, fileOutput, file,tempcmd,temp;
	     size_t found;
	     string request;
	     struct sockaddr_in serv_addr, cli_addr;
	     int msg;
	     portno = 2100;
	     if (portno<1) {
		 cout<<"Error, No Port Provided\n";
		 exit(1);
	     }
	     sockfd = socket(AF_INET, SOCK_STREAM, 0);
	     if (sockfd < 0)
		 error("Socket Error...");
	     memset((char *) &serv_addr,'\0', sizeof(serv_addr));
	     serv_addr.sin_family = AF_INET;
	     serv_addr.sin_addr.s_addr = INADDR_ANY;
	     serv_addr.sin_port = htons(portno);
	     if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		      error("Binding Error..");
	     confd = connectClient(clilen, cli_addr, sockfd);
	     while(1){
         cout<<endl<<"Start File"<<endl;
         memset(buffer,'\0',sizeof(buffer));
         do{
	     msg = recv(confd,buffer,sizeof(buffer),0);
         }while(msg<0);
	     cout<<"Message from Client: \n"<<buffer<<"\n";
	     file = parseRequest(buffer);
	     if(file==" "){
	     request="HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
	     fileOutput = "<html><head><link rel=\"icon\" href=\"data:,\"></head><body>\n";
	     res=exec("dir /b");
	     cmd.str(res);
	     while(getline(cmd,listfile))
	     	fileOutput+="<a href=\""+listfile+"\">"+listfile+"</a><br>\n";
	     request+= to_string(fileOutput.length()) + "\n\n" + fileOutput + "</body></html>";
	     cout<<endl<<request<<endl;
	     msg = send(confd,request.c_str(),request.length(),0);
	     if (msg == SOCKET_ERROR)
            error("Error, no message sent..");
	     listfile.clear();
	     fileOutput.clear();
	     res.clear();
	     cmd.clear();
	     request.clear();
	     }
	     else{
         tempcmd=exec("dir /b/d");
         tempcmd.pop_back();
         found = tempcmd.find(file);
         tempcmd.clear();
         if(found!=string::npos){
            dir=dirExists(file);
         }
         else{
            tempcmd="dir \""+file+"\" /b/a/s";
            temp=exec(tempcmd);
            temp.pop_back();
            cmd.str(temp);
            getline(cmd,path);
            tempcmd.clear();
            temp.clear();
            cmd.clear();
            dir=dirExists(path);
            path.clear();
         }
	     if(dir==false){
            path.clear();
            tempcmd = "dir \""+file+"\" /b/s/a";
            temp=exec(tempcmd);
            temp.pop_back();
            cmd.str(temp);
            getline(cmd,path);
            temp.clear();
            cmd.clear();
            fileRead(path, &fileOutput);
            request.clear();
            request="HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: " + to_string(fileOutput.length()) + "\n\n" + fileOutput;
            cout<<request<<endl;
            msg = send(confd,request.c_str(),request.length(),0);
            if (msg < 0)
                error("Message could not be sent..");
            fileOutput.clear();
            tempcmd.clear();
            path.clear();
            request.clear();
	     }
	     else{
            res=exec("dir /b/d");
            res.pop_back();
            found = res.find(file);
            res.clear();
            if(found!=string::npos){
                tempcmd="dir \""+file+"\" /b";
                res=exec(tempcmd);
                cmd.str(res);
                while(getline(cmd,listfile))
                    fileOutput+="<a href=\""+listfile+"\">"+listfile+"</a><br>\n";
                request ="HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + to_string(fileOutput.length()) + "\n\n" + "<html><head><link rel=\"icon\" href=\"data:,\"></head><body>" + fileOutput + "</body></html>";
                cout<<request<<endl;
                msg = send(confd,request.c_str(),request.length(),0);
                if (msg == SOCKET_ERROR)
                    error("Error, no message sent..");
                listfile.clear();
                fileOutput.clear();
                res.clear();
                cmd.clear();
                tempcmd.clear();
                path.clear();
                request.clear();
            }
            else{
                path.clear();
                tempcmd="dir \""+file+"\" /b/a/s";
                path=exec(tempcmd);
                path.pop_back();
                tempcmd.clear();
                tempcmd="dir "+path+"/b";
                res=exec(tempcmd);
                cmd.str(res);
                while(getline(cmd,listfile))
                    fileOutput+="<a href=\""+listfile+"\">"+listfile+"</a><br>\n";
                request = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + to_string(fileOutput.length()) + "\n\n" + "<html><head><link rel=\"icon\" href=\"data:,\"></head><body>" + fileOutput + "</body></html>";
                cout<<request<<endl;
                msg = send(confd,request.c_str(),request.length(),0);
                if (msg == SOCKET_ERROR)
                    error("Error, no message sent..");
                listfile.clear();
                fileOutput.clear();
                res.clear();
                cmd.clear();
                tempcmd.clear();
                path.clear();
                request.clear();
            }
	     }
	     }
	     }

         closesocket(confd);
         closesocket(sockfd);
	     }
