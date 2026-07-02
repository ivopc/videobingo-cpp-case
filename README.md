# Simulador de Sorteio de Bingo

## Visão geral e histórico do desenvolvedor
Descrição técnica do projeto de case para a vaga de emprego para a FBM Brasil. 
README escrito manualmente por mim e a escolha de arquitetura do sistema também foi influenciada de projetos e experiencias profissionais passadas, uma vez que já desenvolvi maturidade com projetos pessoais fazendo por exemplo uma game engine e framework de RPG e MMOPRG, moldando arquiteturas de trabalhos comerciais desde o inicio, manutenção em sistemas de médio-grande porte, conhecimento de arquitetura e abstração generalista de game engines e frameworks independente da linguagem ou ferramenta e desenvolvimento de software no geral de médio-grande porte.
Não sei se é uma competencia necessária mas sei trabalhar com ferramentas de IA como: context engineering e multi AI agent e qualquer ferramenta como chatgpt, gemini, claude para orquestar e dar um norte em cada execução apesar de também gostar de fazer meus projetos pessoais de forma "artesanal" sem uso direto de IA para digitar código.

## Recursos implementados
- [x] Sorteio de 4 números distintos entre 0 e 9
- [x] Botão JOGAR clicável com feedback visual
- [x] Animação sequencial com delta time
- [x] Botão desabilitado durante animação
- [x] AssetManager com cache de texturas
- [x] Log em sorteio.log
- [x] Histórico em tempo real
- [x] Aspect ratio preservado no resize
- [x] [EXTRA] Data oriented design: Configuração por JSON  
- [x] [EXTRA] Scripting: Camada Lua opcional
- [x] [EXTRA] Camada de testes para cada função isolada no sistema.

## Stack utilizada
- C++ 
- SFML 3.2
- Lua (Sol2)
- nlohmann/json

Complementares:
- Krita (semelhante ao photoshop para deixar fundo das imagens transparentes)
- Leshy SpriteSheet Tool (criar spritesheet e JSON map para os assets de arte do jogo)

## Justificativa da stack
Uma vez que C++, LUA e SFML são as ferramentas principais, então com certeza pensei que seriam as mais adequadas para o desafio tecnico, além de ser leve C++ e SFML permitem um excelente controle da arquitetura onde tenho liberdade e mais trabalho manual para fazer ajustes finos onde em game engines o usuário fica mais amarrado a estrutura já estabelecida. 
Usei LUA para fazer uma pequena camada de scripting em runtime e conseguir controlar alguns eventos e comportamentos do jogo de forma mais prática e como eu venho do desenvolvimento web também achei interessante usar JSON para trabalhar com Data Oriented Design, porque é fácil de ler, se alinha com LUA para o fácil entendimento de game designers e possivelmente lidar com dados do protocolo da internet.
Não muito diferente de outros trabalhos e projetos pessoais que tive, o sistema ficou bem próximo do que trabalhei em outras linguagens em aspectos de arquitetura e organização com a diferença que C++ nos permite ter mais controle e gerenciamento de memória.

## Arquitetura
Pensei em usar a arquitetura de Entity-Component-System e mesmo sendo uma excelente arquitetura acabei pensando em algo mais enxuto e prático sendo que são apenas dois dias para entregar o resultado e boa parte dos lugares que trabalhei com cassino sempre tendiam a usar GameObject sendo que boa parte dos jogos assim são baseados em UI + sprites/modelos 3d animadas além de Entity-Component-System lidar melhor onde há várias instancias rodando ao mesmo tempo.
Devido às minhas experiencias passadas em outros cassino, notei que Data Oriented Design + Orientação a Objeto sempre é uma escolha bem presente e resolvi escolher o equilibrio entre ambas. Usei DOD com JSON para carregar os valores dinâmicos do jogo onde está localizado em `config/game.json` armazenando recursos de layout, nome do jogo, mapeamento de assets e o que é necessário para evitar hard coded data (sabendo separar bem o que fazer para evitar número mágico e hard coded).
Com uso opcional do LUA no projeto usei ele para definir alguns comportamentos simples dentro do sistema de eventos do jogo o que a longo prazo tem potencial para scripting conforme é utilizado em game engines robustas da industria, onde os programadores conseguem colaborar com game designers e outros que produzem mas não são ligados a programação dentro da API do próprio jogo com a simplicidade das linguagens de script. Inclusive eu já havia feito isso em outro projeto de jogo MMORPG turn based meu, mas usando JSON como se fosse uma linguagem de script.

Decisões tomadas por pastas e módulos de cada parte do sistema:

- /src/app/ - Aplicação e coordenação
Orquestradores principais da aplicação no geral: 
Application carrega Config, cria Window, inicializa serviços e sistemas importantes como AssetManager e GameCoordinator, executa e delega game loop e update/render.
GameCoordinator tem o estado atual da aplicação, chama BingoDrawService quando o botão é clicado, cria animações no BallAnimationPool, atualiza HistoryService e LogService, gerencia eventos LUA, controla interação do botão, os eventos são controlados ali.

- /src/domain - Dominio
Responśavel por regras de negócio no sentido mais cru da aplicação, por exemplo: sorteia 4 numeros e garante que sejam diferentes. Por essência não conhece nada do SFML, Sprites, arquivo de recursos, as windows ou scripts lua.

- /src/data - Data oriented design
Dados compactos, fácil de atualizar em loop, menos alocações por frame, simples de reservar capacidade para 4 bolas, como o projeto so anima 4 bolas. 
Exemplo de conceito:
```cpp
struct BallAnimationPool {
    std::vector<int> numbers;
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> startX;
    std::vector<float> targetX;
    std::vector<float> elapsed;
    std::vector<float> duration;
    std::vector<float> delay;
    std::vector<bool> active;
    std::vector<bool> completed;
};
```

- /src/systems - Sistemas
Sistemas estão entre dados e recursos
`AnimationSystem` atualiza posições com delta time, `RenderSystem`: desenha bolas, botão, histórico, `InputSystem`: transforma eventos SFML para ações de aplicação
A ideia de colocar uma lista de alocação dinâmica e generalista dentro do `/src/systems/RenderSystem.cpp` junto com um Factory global foi pensada, mas deixada de lado devido ao pragmatismo e a aplicação não precisar de escalas a longo prazo.

- /src/render - Render
`AssetManager` deixa `sf::Texture` em cache, `SpriteFactory` cria sprites a partir das texturas já carregadas, `ViewportAdapter` preserva aspect ratio, `HistoryPanel` desenha ultimos sorteios.

- /src/infra - Infraestrutura
Controle de dados e recursos internos
`ConfigService`: lê `game.json`, `LogService`: escreve `sorteio.log` em append, `TimeService`: data/hora formatada.

- /src/scripting - Scripting
`IScriptingService`: interface abstrata, `LuaAutomationService`: implementação Lua, `ScriptEvents`: eventos.
Funciona mesmo se Lua estiver desativado no `config/game.json`.. Pois se trata de uma camada do sistema complementar.


## Instruções claras de compilação/execução e gerenciamento de dependências
ToDo..

## Instruções de Execução
ToDo..