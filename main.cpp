#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <math.h>

void MoveSprite(sf::Sprite& target, sf::Vector2f dir, float speed);
bool IsOutOfScreen(sf::Sprite& sprite, sf::Texture texture);
void SpawnSprite(sf::Sprite& sprite, sf::Texture texture, sf::Vector2f& dir, float posY);

enum class Side { LEFT, RIGHT, NONE };

float deltaTime;
float curTime = 0;
int randomX = 0;
int randomY = 0;
float nextActionTime = 0;

int main()
{
    // 창 설정
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");

    srand((int)time(0));


    // 텍스쳐 설정
    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");

    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");

    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");

    sf::Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");

    sf::Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");


    // 스프라이트
    sf::Sprite spriteBackground; // 배경        
    spriteBackground.setTexture(textureBackground);

    sf::Sprite spriteCloud[3];  // 구름
    for (int i = 0; i < 3; i++)
        spriteCloud[i].setTexture(textureCloud);

    sf::Sprite spriteTree;      // 나무
    spriteTree.setTexture(textureTree);

    sf::Sprite spriteBee[2];    // 벌
    for (int i = 0; i < 2; i++)
        spriteBee[i].setTexture(textureBee);

    sf::Sprite spritePlayer;    // 플레이어
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setOrigin(texturePlayer.getSize().x * 0.5f + textureTree.getSize().x * -0.5f - 100, texturePlayer.getSize().y);
    spritePlayer.setPosition(1920 * 0.5, 950);
    Side sidePlayer = Side::LEFT;
    
    
                                // 나무
    const int NUM_BRANCHES = 6; // const 키워드 읽기만 가능, 쓰기 불가능 → 즉 수정 불가 (상수)    
    sf::Sprite spriteBranch[NUM_BRANCHES];
    Side sideBranch[NUM_BRANCHES] = { Side::LEFT, Side::RIGHT, Side::NONE, Side::LEFT, Side::RIGHT, Side::NONE };
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        spriteBranch[i].setTexture(textureBranch);
        spriteBranch[i].setOrigin(textureTree.getSize().x * -0.5f, 0);
        spriteBranch[i].setPosition(1920 * 0.5f, i * 150.f);
    }

    // 나무 생성
    spriteTree.setOrigin(textureTree.getSize().x * 0.5f, 0.0f);
    spriteTree.setPosition(1920 * 0.5f, 0.0f);

    // 구름 생성
    float cloudSpeed[3];    
    sf::Vector2f cloudDir[3];
    for (int i = 0; i < 3; i++)
    {
        // 속도
        int random = rand() % 9;
        cloudSpeed[i] = random * 30.0f + 50.0;

        // 생성
        SpawnSprite(spriteCloud[i], textureCloud, cloudDir[i], textureCloud.getSize().y * i );
    }

    // 벌 생성
    sf::Vector2f beeDir[2];
    float beeSpeed[2] = { 200, 200 };
    sf::Vector2f beeVec[2];
    for (int i = 0; i < 2; i++)
    {
        spriteBee[i].setOrigin(textureBee.getSize().x * 0.5f, textureBee.getSize().y * 0.5f);
    }

    spriteBee[0].setPosition(500, 500);
    SpawnSprite(spriteBee[1], textureBee, beeDir[1], 500);
        
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time time = clock.restart(); // 시간 초기화까지 누적된 시간 반환
        deltaTime = time.asSeconds();

        // 이벤트 루프
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
       
        // 구름 이동
        for (int i = 0; i < 3; i++)
        {
            // 화변 밖으로 나갈 경우 재 생성
            if (IsOutOfScreen(spriteCloud[i], textureCloud))
            {
                SpawnSprite(spriteCloud[i], textureCloud, cloudDir[i], textureCloud.getSize().y * i);


            }

            // 이동
            MoveSprite(spriteCloud[i], cloudDir[i], cloudSpeed[i]);
        }

        // 벌 이동 (랜덤 이동)
        sf::Vector2f beePos0 = spriteBee[0].getPosition();

        curTime += deltaTime;

        if (curTime >= nextActionTime)
        {
            randomX = rand() % 3 - 1;
            randomY = rand() % 3 - 1;

            nextActionTime = curTime + 1.5f;
        }

        sf::Vector2f beeDir0 = { (float)randomX , (float)randomY };
        MoveSprite(spriteBee[0], beeDir0, beeSpeed[0]);


        // 벌 이동 (포물선 운동)
        sf::Vector2f beePos1 = spriteBee[1].getPosition();
        float rd = beePos1.x * 3.14f / 180;
        beePos1 += beeDir[1] * beeSpeed[1] * deltaTime;
        beePos1.y = (sin(rd) * 50) + 800;
        spriteBee[1].setPosition(beePos1);


        // 벌 화면 밖으로 나갈 경우 재 생성
        for (int i = 0; i < 2; i++)
        {
            if (IsOutOfScreen(spriteBee[i], textureBee))
            {
                SpawnSprite(spriteBee[i], textureBee, beeDir[i], 600);
            }
        }

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            switch (sideBranch[i])
            {
                case Side::LEFT:
                    spriteBranch[i].setScale(-1.f, 1.f);
                    break;

                case Side::RIGHT:
                    spriteBranch[i].setScale(1.f, 1.f);
                    break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            sidePlayer = Side::LEFT;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            sidePlayer = Side::RIGHT;
        }

        switch (sidePlayer)
        {
            case Side::LEFT:
                spritePlayer.setScale(-1, 1);
                break;

            case Side::RIGHT:
                spritePlayer.setScale(1, 1);
                break;
        }

        // 화면에 출력
        window.clear();

        window.draw(spriteBackground); // 배경

        for (int i = 0; i < 3; i++) // 구름
        {
            window.draw(spriteCloud[i]);
        }

        window.draw(spriteTree); // 나무 기둥

        for (int i = 0; i < NUM_BRANCHES; i++) // 나뭇가지
        {
            if (sideBranch[i] != Side::NONE)
            {
                window.draw(spriteBranch[i]);
            }
        }

        for (int i = 0; i < 2; i++) // 벌
        {
            window.draw(spriteBee[i]);
        }

        window.draw(spritePlayer); // 플레이어
        
        

        window.display();

        
    }

    return 0;
}

// 스프라이트 이동
void MoveSprite(sf::Sprite& target, sf::Vector2f dir, float speed)
{
    // 이동
    sf::Vector2f pos = target.getPosition();
    pos += dir * speed * deltaTime;
    target.setPosition(pos);

    // 이동 방향에 따라 바라보는 방향 변경
    if (dir.x < 0)
    {
        target.setScale(1, 1);
    }
    else
    {
        target.setScale(-1, 1);
    }
}

// 스프라이트가 화면 밖으로 나갔는지 여부 확인
bool IsOutOfScreen(sf::Sprite& sprite, sf::Texture texture)
{
    float textureSizeX = texture.getSize().x;
    if (sprite.getPosition().x < -textureSizeX || sprite.getPosition().x > textureSizeX + 1920)
    {
        return true;
    }
    float textureSizeY = texture.getSize().y;
    if (sprite.getPosition().y < -textureSizeY || sprite.getPosition().y > textureSizeY + 1080)
    {
        return true;
    }

    return false;
}

// 스프라이트 생성
void SpawnSprite(sf::Sprite& sprite, sf::Texture texture, sf::Vector2f& dir, float posY)
{
    // 랜덤 방향 설정
    float random = (float)rand() / RAND_MAX;
    float textureSizeX = texture.getSize().x;

    if (random < 0.5f)
    {
        dir.x = 1.0f; // 이동 방향 지정
        sprite.setPosition(-textureSizeX * 2, posY); // 생성
    }
    else
    {
        dir.x = -1.0f;
        sprite.setPosition(textureSizeX + 1920, posY);
    }
}


