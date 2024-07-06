void mqttSetup() {
    if (!data.isMqttEnabled) return;
    if (strlen(data.mqttHost) == 0) return;
    hub.mqtt.config(data.mqttHost, data.mqttPort, data.mqttUser, data.mqttPass);
}
