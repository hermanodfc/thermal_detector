# Thermal Detector


Um aplicativo feito em Flutter, baseado no projeto SnakeEye, que permite a comunicação entre Smartphones e sensores MLX90640 acoplados em uma placa ESP32. O aplicativo tem por objetivo simplicar a coleta de dados térmicos em experimentos realizados em regiões sem rede de comunicação disponível. Os dados capturados serão utilizados durante a realização da pesquisa de mestrado do desenvolvedor deste projeto na UFRPE.

- App desenvolvido como projeto da Disciplina Fundamentos de Informática Aplicada da UFRPE/PPGIA.

### Requisitos:

- Placa ESP32 (ou compatível) com WiFi;
- Sensor MLX90640.
- Smarthphone compatível

### Características:   

- Permite realizar a construção de uma imagem térmica a partir dos dados capturados, os quais serão salvos, no formato TXT, para uso posterior;
- Elimina a necessidade de compilação do software embarcado quando for necessário fazer ajustes nos parâmetros de configuração do sensor.

### Widget tree e Telas

<img src="https://github.com/hermanodfc/thermal_detector/blob/master/images/WidgetTree1.png" width="487" height="339">
<img src="https://github.com/hermanodfc/thermal_detector/blob/master/images/WidgetTree2.png" width="682" height="378">
<img src="https://github.com/hermanodfc/thermal_detector/blob/master/images/WidgetTree3.png" width="481" height="466">
