# Relatório — Sistema de Monitorização e Alarmes

**Unidade curricular:** Sistemas de Tempo Real — Projeto, Parte 1 (PIC)  
**Grupo:**   
**Elementos:** 
**Plataforma:** Curiosity High Pin Count, PIC16F18875

## 1. Objetivo e organização

O projeto implementa um sistema embebido de um alarme, com monitorização de horário, temperatura e luminosidade, de acordo com o enunciado da Parte 1. Foi desenvolvida uma solução em C sem sistema operativo, adequada aos recursos do microcontrolador. O código está dividido por responsabilidade: controlo e integração em [main.c](./main.c), interface em [UI/ui.c](./UI/ui.c), relógio em [Time/Time.c](./Time/Time.c), acesso à EEPROM em [EEPROM/config_storage.c](./EEPROM/config_storage.c), comunicação I²C em [I2C/i2c.c](./I2C/i2c.c) e controlo do LCD em [LCD/lcd.c](./LCD/lcd.c). A configuração dos periféricos e os respetivos drivers foram gerados pelo MPLAB Code Configurator.

## 2. Decisões técnicas

- **Execução e tempo real:** Adotou-se um ciclo principal que coloca o microcontrolador em `SLEEP()` e retoma após interrupções. O Timer1 fornece a base temporal para o relógio (1 segundo) e para a amostragem periódica (5 segundos). As interrupções dos botões S1 e S2 assinalam interação do utilizador. O Timer5 controla a duração dos ecrãs temporários da interface. Esta combinação de `SLEEP()` e interrupções poupa energia ao evitar manter o processador em idle.
- **Sensores:** A temperatura é lida do TC74 através de I²C. A luminosidade é simulada pelo potenciómetro e convertida pelo ADC em oito níveis, de 0 a 7, usando a escala `ADC × 7 / 1023`. A amostragem está atualmente configurada para cada cinco segundos, por meio de `PMON_DEFAULT`.
- **Interface:** A máquina de estados em `ui.c` separa o ecrã normal, a consulta de limiares e registos e os vários passos de configuração. S1 avança ou seleciona operações e S2 incrementa valores ou alterna opções. A comunicação com o LCD de 16 × 2 caracteres é também realizada através de I²C. Os LED indicam atividade do relógio e estados de alarme.
- **Persistência:** A EEPROM interna guarda os parâmetros e a hora (horas e minutos). A validade dos dados é verificada com a palavra mágica `0xA5` e um checksum de um byte. Em caso de dados inválidos, são repostos valores predefinidos. Os máximos e mínimos são mantidos em estruturas em RAM, com o valor e o instante da ocorrência.
- **Alarmes:** As notificações de relógio, temperatura e luminosidade são representadas por uma máscara de bits e apresentadas no LCD. O Timer2 e um contador associado controlam a duração do sinal de alarme. Os LED de temperatura e luminosidade refletem os respetivos limiares. A configuração `ALAF` ativa ou desativa os alarmes.

## 3. Interação entre componentes

No arranque, são inicializados os periféricos, a interface, a EEPROM e os handlers de interrupção. Em funcionamento, a interrupção periódica atualiza o relógio, lê os sensores (se for o caso) e verifica as condições de alarme. O ciclo principal recolhe os dados para apresentação, atualiza a interface quando há uma interrupção ou expira um ecrã temporário e persiste os parâmetros. Assim, as interrupções fornecem a referência temporal e acordam o ciclo principal, enquanto a máquina de estados concentra a interface de apresentação e configuração.

## 5. Conclusão

O projeto integra num único sistema embebido as funções de relógio, monitorização de temperatura e luminosidade, alarmes e interação local, recorrendo aos periféricos do PIC16F18875 e sem sistema operativo. A organização modular, a utilização de interrupções e o modo `SLEEP()` respondem aos objetivos de estruturação e eficiência energética do enunciado. A teste do programa em hardware físico foi importante para confirmar o comportamento temporal e o funcionamento dos periféricos.
