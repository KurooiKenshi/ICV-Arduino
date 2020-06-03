# Alimentador para animais de estimação com Arduino

Utilizando a plataforma Arduino, foi desenvolvido o controlador para o alimentador de animais de estimação. Junto com a placa NodeMCU, estão os componentes de motor, para controlar a porta que permite a passagem da ração, relógio, para verificar os horários em que a porta deve ser aberta, e balança, para controlar a quantidade de ração liberada.

<p align="center" style="text-align: center;">
  <img src="https://i.imgur.com/0G148m3.png" alt="Alimentador Fritzing" title="Circuito do controlador do alimentador com todos os         componentes." height = 400px/>
</p>


Durante a realização deste trabalho, foi desenvolvido um alimentador de animais completamente funcional que pode ser programado para liberar ração em horários determinadas e em quantidades reguladas. Além disso, futuramente, o alimentador deverá possuir ferramentas para facilitar a configuração dos horários e quantidades de ração. Essa interface com o usuário pode ser feita através de um leitor de cartão SD, que deve conter um arquivo com as configurações, ou por um aplicativo online.

Finalmente, o controlador foi acoplado à uma estrutura física, que permite a armazenagem da ração em um compartimento e libera a comida nos horários definidos. A ração liberada cai sobre um recipiente posicionado em cima da balança, onde o peso é controlado. Para que as quantias liberadas pudessem ser medidas com a precisão necessária pela balança, a porta foi ajustada para abrir e fechar repetidamente até que a quantidade fornecida seja adequada, dessa forma, a comida é liberada em pequenas quantidades entre as aberturas da porta, impedindo que quantidades exageradas sejam despejadas.

As imagens a seguir mostram o resultado final do alimentador.

<p align="center" style="text-align: center;">
  <img src="https://i.imgur.com/mQK5DjE.png" alt="Alimentador Componentes" title="Extrutura do alimentador com todos os componentes anexados." width = 600px/>
</p>

<p align="center" style="text-align: center;">
  <img src="https://i.imgur.com/VuWaTVU.png" alt="Alimentador Funcionando" title="Alimentador despejando ração." width = 600px/>
</p>

<p align="center" style="text-align: center;">
  <img src="https://i.imgur.com/vYq06LI.png" alt="Alimentador Completo" title="Alimentador completo, em sua versão final." width = 600px/>
</p>

