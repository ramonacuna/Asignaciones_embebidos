#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <time.h>

 int main(){
 unsigned long long NTP_TIMESTAMP_DELTA = 2208988800;
 int conection,socket_id;

 // Creacion del socket
   socket_id = socket(AF_INET,SOCK_DGRAM,0);
 // Creacion de puerto
   struct sockaddr_in server_ntp;
   server_ntp.sin_family = AF_INET;
   server_ntp.sin_addr.s_addr = inet_addr("45.11.105.123");
   server_ntp.sin_port = htons(123);
 
 //protocolo NTP
 typedef struct {
 unsigned char li_vn_mode;// 8 bits
 unsigned char stratum;
 unsigned char poll;// 8 bits
 unsigned char precision;// 8 bits

 unsigned int rootDelay;//32 bits
 unsigned int rootDispersion;//32 bits
 unsigned int refId;//32 bits

 unsigned int refTm_s;
 unsigned int refTm_f;//64 bits

 unsigned int origTm_s;
 unsigned int origTm_f;//64 bits

 unsigned int rxTm_s;
 unsigned int rxTm_f;//64 bits

 unsigned int txTm_s;
 unsigned int txTm_f;//64 bits
 } ntp_packet;// Total: 384 bits 
 
 ntp_packet packet = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 *( ( char * ) &packet) = 00011011;//li = 0, vn = 3, and mode = 3.
 
 //Establesco la coneccion 
 conection=connect(socket_id,(struct sockaddr *)&server_ntp,sizeof(server_ntp));
 if(conection < 0){
 printf("Coneccion Fallida\n\r");
 return(-1);
 }
 //Envio peticion y leo 
 send(socket_id,(char *)&packet,sizeof(ntp_packet),0);
 recv(socket_id, (char *)&packet, sizeof(ntp_packet),0);
 time_t txTm = ntohl(packet.txTm_s)-NTP_TIMESTAMP_DELTA;

 printf("Tiempo:%s",ctime((time_t *)&txTm));
    close(socket_id);
    return(0);
}