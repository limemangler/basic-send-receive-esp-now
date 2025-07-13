#include <esp_now.h>
#include <WiFi.h>

typedef struct pkt_struct {
  // message
  char message[200] = "this is a lime service announcement!";
} struct_message;

pkt_struct rx_packet;
pkt_struct tx_packet;
esp_now_peer_info_t peerInfo;

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(PacketSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("Peer Added!!!");
  }
 
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(PacketReceived));
}

void loop() {
  // nothing to see here
  // tx_packet.message = "this is a lime service announcement!";
  esp_now_send(broadcastAddress, (uint8_t *) &tx_packet, sizeof(tx_packet));
  delay(10000);
}
void PacketReceived(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&rx_packet, incomingData, sizeof(rx_packet));
  Serial.print("Packet received: ");
  Serial.println(rx_packet.message);
}

void PacketSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}