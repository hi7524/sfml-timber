#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>


int windowSizeW = 1920;
int windowSizeH = 1080;

float deltaTime;
float curTime = 0;

enum class Side
{
    LEFT,
    RIGHT,
    NONE
};


void Movement(sf::Sprite& sprite, sf::Vector2f dir, float speed);
bool IsOutOfScreen(sf::Sprite& sprite, sf::Texture spriteTexture);
int RandomInt(int num1, int num2);
bool RandomBool();
void UpdateBranches(Side* branchSide, int size);
void SetTextOrigin(sf::Text& text);


int main()
{
    // 창 생성
    sf::RenderWindow window(sf::VideoMode(windowSizeW, windowSizeH), "TimberRemake");

    srand((int)time(0)); // 난수 생성

    // Texture
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("graphics/background.png");
    sf::Texture cloudTexture;
    cloudTexture.loadFromFile("graphics/cloud.png");
    sf::Texture treeTexture;
    treeTexture.loadFromFile("graphics/tree.png");
    sf::Texture branchTexture;
    branchTexture.loadFromFile("graphics/branch.png");
    sf::Texture playerTexture;
    playerTexture.loadFromFile("graphics/player.png");
    sf::Texture logTexture;
    logTexture.loadFromFile("graphics/log.png");
    sf::Texture beeTexture;
    beeTexture.loadFromFile("graphics/bee.png");
    

    // Font
    sf::Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    // SoundBuffer
    sf::SoundBuffer bufferChop;
    bufferChop.loadFromFile("sound/chop.wav");
    sf::SoundBuffer bufferDeath;
    bufferDeath.loadFromFile("sound/death.wav");
    sf::SoundBuffer bufferOutOfTime;
    bufferOutOfTime.loadFromFile("sound/out_of_time.wav");

    // Sound
    sf::Sound soundChop;
    soundChop.setBuffer(bufferChop);
    sf::Sound soundDeath;
    soundDeath.setBuffer(bufferDeath);
    sf::Sound soundOutOfTime;
    soundOutOfTime.setBuffer(bufferOutOfTime);

    // Sprite
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Sprite cloudSprite[3];
    for (int i = 0; i < 3; i++)
        cloudSprite[i].setTexture(cloudTexture);

    sf::Sprite treeSprite;
    treeSprite.setTexture(treeTexture);

    const int branchNum = 6;
    sf::Sprite branchSprite[branchNum];
    for (int i = 0; i < branchNum; i++)
        branchSprite[i].setTexture(branchTexture);

    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);

    sf::Sprite beeSprite[2];
    for (int i = 0; i < 2; i++)
        beeSprite[i].setTexture(beeTexture);

    const int logNum = 10;
    sf::Sprite logSprite[logNum];
    for (int i = 0; i < logNum; i++)
        logSprite[i].setTexture(logTexture);


    // 구름 초기 설정
    float cloudSpeed[3];
    sf::Vector2f cloudDir[3];
    for (int i = 0; i < 3; i++)
    {
        cloudSpeed[i] = RandomInt(1, 5) * 100.0f; // 속도 설정

        if (RandomBool()) // 이동 방향 설정
        {
            cloudDir[i] = { -1, 0 };
            cloudSprite[i].setPosition(windowSizeW + cloudTexture.getSize().x, cloudTexture.getSize().y * i);
        }
        else
        {
            cloudDir[i] = { 1, 0 };
            cloudSprite[i].setPosition(-(int)cloudTexture.getSize().x, cloudTexture.getSize().y * i);
        }
    }

    // 나무 기둥 초기 설정
    treeSprite.setOrigin(treeTexture.getSize().x * 0.5f, 0);
    treeSprite.setPosition(windowSizeW * 0.5f, 0);
    
    // 나뭇가지 초기 설정
    Side branchSide[branchNum];
    for (int i = 0; i < branchNum; i++)
    {
        branchSprite[i].setPosition(0, i * 150);
        branchSprite[i].setOrigin(treeTexture.getSize().x * -0.5f, 0);
        branchSprite[i].setPosition(windowSizeW * 0.5f, i * 150);

        int random = RandomInt(0, 3);
        if (random == 0) 
        {
            branchSide[i] = Side::LEFT;
        }
        else if (random == 1)
        {
            branchSide[i] = Side::RIGHT;
        }
        else
        {
            branchSide[i] = Side::NONE;
        }

        branchSide[branchNum - 1] = Side::NONE; // 초기 설정시 마지막 나뭇가지는 비어있도록
    }

    // 플레이어 초기 설정
    playerSprite.setOrigin(playerTexture.getSize().x * 0.5f, playerTexture.getSize().y);
    playerSprite.setPosition(treeSprite.getPosition().x + treeTexture.getSize().x * 0.5f + 150, 950);
    Side playerSide = Side::RIGHT;

    // 나무 초기 설정
    sf::Vector2f logInitPosition = treeSprite.getPosition();
    logInitPosition.y = treeTexture.getSize().y;

    bool isActiveLog[logNum];

    sf::Vector2f logDirR = { 1.f, -1.f };
    sf::Vector2f logDirL = { -1.f, -1.f };
    sf::Vector2f logVelocity[logNum];

    float logSpeed = 2000.0f;
    sf::Vector2f gravity = { 0.f, 6000.f };
    
    for (int i = 0; i < logNum; i++)
    {
        logSprite[i].setOrigin(logTexture.getSize().x * 0.5f, logTexture.getSize().y);
        logSprite[i].setPosition(logInitPosition);
        isActiveLog[i] = false;
    }

    // 벌 초기 설정
    for (int i = 0; i < 2; i++)
    {
        beeSprite[i].setOrigin(beeTexture.getSize().x * 0.5, beeTexture.getSize().y);
    }
    float beeSpeed = 300;
    beeSprite[0].setPosition(500, 500);

    
    // UI
    sf::Text scoreText; // 점수 텍스트
    scoreText.setFont(font);
    scoreText.setString("SCORE: 0");
    scoreText.setCharacterSize(80);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 20);

    sf::Text infoText; // 게임 정보 텍스트
    infoText.setFont(font);
    infoText.setString("- Enter to START -");
    infoText.setCharacterSize(100);
    infoText.setFillColor(sf::Color::White);
    SetTextOrigin(infoText);

    sf::RectangleShape timeBar; // 시간바
    float timeBarWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize({ timeBarWidth, timeBarHeight });
    timeBar.setFillColor(sf::Color::Red);
    timeBar.setPosition(windowSizeW * 0.5f - timeBarWidth * 0.5f, windowSizeH - 100.0f);


    // 게임 데이터
    sf::Clock clock;

    bool isPlaying = false;
    bool isPlayerDead = false;
    bool isLeft = false;
    bool isRight = false;
    int score = 0;
    float remaingTime = 5.0f;
    float timeBarWidthPerSecond = timeBarWidth / 5.0f;
    float nextActionTime = 0;
    int logIdx = 0;
    int randomX = 0;
    int randomY = 0;

    while (window.isOpen())
    {
        sf::Time time = clock.restart();
        deltaTime = time.asSeconds();

        // 이벤트 루프
        sf::Event event;

        bool isLeftDown = false;
        bool isLeftUp = false;
        bool isRightDown = false;
        bool isRightUp = false;

        while (window.pollEvent(event))
        {
            // 종료
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            // 키 입력
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Left:
                    if (!isLeft)
                    {
                        isLeftDown = true;
                    }
                    isLeft = true;
                    break;
                case sf::Keyboard::Right:
                    if (!isRight)
                    {
                        isRightDown = true;
                    }
                    isRight = true;
                    break;
                case sf::Keyboard::Return:
                    isPlaying = !isPlaying;

                    if (isPlayerDead)
                    {
                        isPlayerDead = false;
                        remaingTime = 5.0f;
                        score = 0;
                        scoreText.setString("SCORE: " + std::to_string(score));
                        branchSide[branchNum - 1] = Side::NONE;

                        for (int i = 0; i < logNum; i++)
                        {
                            isActiveLog[i] = false;
                            logSprite[i].setPosition(logInitPosition);
                        }
                        
                    }
                    else
                    {
                        infoText.setString("Enter to Resume!");
                        SetTextOrigin(infoText);
                    }
                    break;
                }
            }
            // 키 뗐을 때
            if (event.type == sf::Event::KeyReleased)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Left:
                    isLeft = false;
                    break;
                case sf::Keyboard::Right:
                    isRight = false;
                    break;
                }
            }
        }

        if (isPlaying)
        {
            // 남은 시간 계산
            remaingTime -= deltaTime;
            if (remaingTime <= 0)
            {
                isPlaying = false;
                isPlayerDead = true;
                infoText.setString("Enter to Restart!");
                SetTextOrigin(infoText);
                soundOutOfTime.play();
            }
            timeBar.setSize({ timeBarWidthPerSecond * remaingTime, timeBarHeight });

            // 키 입력시
            if (isLeftDown || isRightDown)
            {
                UpdateBranches(branchSide, branchNum);

                if (isLeft)
                {
                    playerSide = Side::LEFT;
                    isActiveLog[logIdx] = true;
                    logVelocity[logIdx] = logDirR * logSpeed;
                    
                }
                else
                {
                    playerSide = Side::RIGHT;
                    isActiveLog[logIdx] = true;
                    logVelocity[logIdx] = logDirL * logSpeed;
                }

                logIdx++;
                if (logIdx > logNum - 1)
                {
                    logIdx = 0;
                }

                // 충돌 검사
                if (playerSide == branchSide[branchNum - 1])
                {
                    isPlaying = false;
                    isPlayerDead = true;
                    infoText.setString("Enter to Restart!");
                    SetTextOrigin(infoText);
                    soundDeath.play();
                }
                else
                {
                    score += 10;
                    scoreText.setString("SCORE: " + std::to_string(score));
                    soundChop.play();
                }
            }

            // 구름 움직임
            for (int i = 0; i < 3; i++)
            {
                Movement(cloudSprite[i], cloudDir[i], cloudSpeed[i]);

                if (IsOutOfScreen(cloudSprite[i], cloudTexture))
                {
                    if (RandomBool()) // 이동 방향 설정
                    {
                        cloudDir[i] = { -1, 0 };
                        cloudSprite[i].setPosition(windowSizeW + cloudTexture.getSize().x, cloudTexture.getSize().y * i);
                    }
                    else
                    {
                        cloudDir[i] = { 1, 0 };
                        cloudSprite[i].setPosition(-(int)cloudTexture.getSize().x, cloudTexture.getSize().y * i);
                    }
                }
            }

            // 나뭇가지 방향에 따른 Scale변경
            for (int i = 0; i < branchNum; i++)
            {
                if (branchSide[i] == Side::LEFT)
                {
                    branchSprite[i].setScale(-1, 1);
                }
                if (branchSide[i] == Side::RIGHT)
                {
                    branchSprite[i].setScale(1, 1);
                }
            }

            // 플레이어 방향에 따른 위치 및 Scale변경
            if (playerSide == Side::LEFT)
            {
                playerSprite.setScale(-1, 1);
                playerSprite.setPosition(treeSprite.getPosition().x + treeTexture.getSize().x * -0.5f - 150, 950);
            }
            else
            {
                playerSprite.setScale(1, 1);
                playerSprite.setPosition(treeSprite.getPosition().x + treeTexture.getSize().x * 0.5f + 150, 950);
            }

            // 나무 움직임
            for (int i = 0; i < logNum; i++)
            {
                // 활성화 된 나무의 위치 계산 및 설정
                if (isActiveLog[i])
                {
                    logVelocity[i] += gravity * deltaTime;

                    sf::Vector2f position = logSprite[i].getPosition();
                    position += logVelocity[i] * deltaTime;
                    logSprite[i].setPosition(position);
                }

                // 화면 밖으로 나갈 경우 재 설정
                if (IsOutOfScreen(logSprite[i], logTexture))
                {
                    isActiveLog[i] = false;
                    logSprite[i].setPosition(logInitPosition);
                }
            }

            // 벌 움직임 (무작위)
            sf::Vector2f beePos0 = beeSprite[0].getPosition();

            curTime += deltaTime;

            if (curTime >= nextActionTime)
            {
                randomX = rand() % 3 - 1;
                randomY = rand() % 3 - 1;

                nextActionTime = curTime + 1.5f;
            }

            sf::Vector2f beeDir0 = { (float)randomX , (float)randomY };

            // 밖으로 나갈 경우 랜덤 위치에 재설정
            if (IsOutOfScreen(beeSprite[0], beeTexture))
            {
                bool random = RandomBool();
                int randomHeight = RandomInt(0, 1080);
                if (random)
                {
                    beeSprite[0].setPosition(beeTexture.getSize().x * -1, randomHeight);
                    beeDir0 = { 1, 0 };
                }
                else
                {
                    beeSprite[0].setPosition(beeTexture.getSize().x + windowSizeW, randomHeight);
                    beeDir0 = { -1, 0 };
                }
            }

            Movement(beeSprite[0], beeDir0, beeSpeed);

            // 벌 이동 (삼각함수)
            sf::Vector2f beePos1 = beeSprite[1].getPosition();
            sf::Vector2f beeDir1 = { -1, 0 };
            float rd = beePos1.x * 3.14f / 180;
            beePos1 += beeDir1 * beeSpeed * deltaTime;
            beePos1.y = (sin(rd) * 50) + 800;
            beeSprite[1].setPosition(beePos1);

            if (IsOutOfScreen(beeSprite[1], beeTexture))
            {
                beeSprite[1].setPosition(windowSizeW + beeTexture.getSize().x, 500);
            }
        }
        
        // 그리기
        window.clear();

        window.draw(backgroundSprite);
        for (int i = 0; i < 3; i++)
            window.draw(cloudSprite[i]); 
        window.draw(treeSprite); 
        for (int i = 0; i < branchNum; i++) 
        {
            if (branchSide[i] != Side::NONE)
            {
                window.draw(branchSprite[i]);
            }
        }
        window.draw(playerSprite);
        for (int i = 0; i < logNum; i++)
        {
            if (isActiveLog[i])
            {
                window.draw(logSprite[i]);
            }
        }
        for (int i = 0; i < 2; i++)
            window.draw(beeSprite[i]);

        window.draw(scoreText);
        window.draw(timeBar);
        if (!isPlaying)
        {
            window.draw(infoText);
        }
        
        window.display();
    }

    return 0;
}

// 스프라이트 이동
void Movement(sf::Sprite& sprite, sf::Vector2f dir, float speed)
{
    sf::Vector2f spritePos = sprite.getPosition();
    spritePos += dir * speed * deltaTime;
    sprite.setPosition(spritePos);

    if (dir.x > 0)
    {
        sprite.setScale(-1, 1);
    }
    else
    {
        sprite.setScale(1, 1);
    }
}

// 스프라이트가 화면 밖으로 나갔는지 여부 확인
bool IsOutOfScreen(sf::Sprite& sprite, sf::Texture spriteTexture)
{
    sf::Vector2f spritePos = sprite.getPosition();
    int spriteSize;

    // 좌, 우
    spriteSize = spriteTexture.getSize().x;
    if (spritePos.x < -spriteSize || windowSizeW + spriteSize < spritePos.x)
    {
        return true;
    }

    // 상, 하
    spriteSize = spriteTexture.getSize().y;
    if (spritePos.y < -spriteSize || windowSizeH + spriteSize < spritePos.y)
    {
        return true;
    }

    return false;
}

// 난수 생성 (num1 이상 num2 미만 숫자중 랜덤 반환)
int RandomInt(int num1, int num2)
{
    int random = rand() % num2 + num1;
    return random;
}

// 랜덤 bool값 반환
bool RandomBool()
{
    float random = (float)rand() / RAND_MAX;

    if (random < 0.5f)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// 나뭇가지 방향 업데이트
void UpdateBranches(Side* branches, int size)
{
    for (int i = size - 1; i > 0; i--)
    {
        branches[i] = branches[i - 1];
    }

    int random = RandomInt(0, 3);
    if (random == 0)
    {
        branches[0] = Side::LEFT;
    }
    else if (random == 1)
    {
        branches[0] = Side::RIGHT;
    }
    else
    {
        branches[0] = Side::NONE;
    }
}

// 메세지 중앙
void SetTextOrigin(sf::Text& text)
{
    sf::Vector2f textOrigin;
    textOrigin.x = text.getLocalBounds().width * 0.5f;
    textOrigin.y = text.getLocalBounds().height * 0.5f;
    text.setOrigin(textOrigin);
    text.setPosition(windowSizeW * 0.5f, windowSizeH * 0.5f);
}