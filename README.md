# EmbarcaTech_Interfaces_ControleSerial
Tarefa do EmbarcaTech com intuito de consolidar os conceitos sobre o uso de interfaces de comunicação serial no RP2040 e explorar as funcionalidades da placa de desenvolvimento BitDogLab.

# Vídeo Explicativo
A seguir um vídeo explicativo sobre o projeto.

"link"

# Intruções
Para executar o código em questão, deve-se possuir a extensão Wokwi Simulator instalada no Visual Studio Code ou acesso a placa BitDogLab configurada para receber informação via Seria Monitor.

Ao ser executado, pressionando o Botão A, o LED de cor verde será ligado ou desligado e pressionando o Botão B, o LED azul será ligado ou desligado, com uma mensagem informativa sobre a ação realizada sendo exibida no display presente na placa e no Serial Monitor presente no Visual Studio Code.

Agora, utilizando o Serial Monitor, ao começar o monitoramento, será póssivel enviar caracteres, de A-Z, a-z e 0-9. Cada caracter enviado será exibido no display conectado a placa e também no Serial Monitor, se enviados números entre 0 e 9, além de exibidos nos meios já mencionados, serão também desenhados na matriz de LED 5x5.

# Composição do código
**Interfaces_ControleSerial.c**
- `main()`: Lógica que configura e inicia o programa.
- `button_isr(uint gpio, uint32_t events)`: Detecta e registra pressionamentos válidos dos botões A e B, evitando leituras incorretas devido a ruídos (debouncing).
- `setup()`: Prepara os pinos dos botões e do LED para uso, configurando entradas com interrupções, e inicializa e configura o I2C e o display.

**led_matriz.c**
- `matrix_rgb(double r, double g, double b)`: Converte valores RGB em um valor de 32 bits no formato específico da matriz.
- `desenho_pio(double *desenho, PIO pio, uint sm, double r, double g, double b)`: Lógica para desenhar o número na matriz de LED 5x5.
- `void imprimir_num(char caracter, PIO pio, uint sm)`: Detecta qual número recebido e o desenha na matriz5x5.

**ssd1306.c**
- Implementa a biblioteca para controlar um display OLED SSD1306 via I2C.

# Desenvolvedor
Abel José Rocha Barros Bezerra