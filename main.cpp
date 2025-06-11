#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>
#include <math.h>

void MoveSprite(sf::Sprite& target, sf::Vector2f dir, float speed);

float deltaTime;
float curTime = 0;
int randomX = 0;
int randomY = 0;
float nextActionTime = 0;
int main()
{
    // â ����
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");

    srand((int)time(0));


    // �ؽ��� ����
    sf::Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");

    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");

    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");


    // ��������Ʈ
    sf::Sprite spriteBackground; // ���        
    spriteBackground.setTexture(textureBackground);

    sf::Sprite spriteCloud[3];  // ����
    for (int i = 0; i < 3; i++)
    {
        spriteCloud[i].setTexture(textureCloud);
    }

    sf::Sprite spriteTree; // ����
    spriteTree.setTexture(textureTree);

    sf::Sprite spriteBee[2];  // ��
    for (int i = 0; i < 2; i++)
    spriteBee[i].setTexture(textureBee);


    // �ʱ� ����

    // ����
    spriteTree.setOrigin(textureTree.getSize().x * 0.5f, 0.0f);
    spriteTree.setPosition(1920 * 0.5f, 0.0f);

    // ����
    for (int i = 0; i < 3; i++)
    {
        spriteCloud[i].setPosition(0.0f, textureCloud.getSize().y * i);
    }

    // ��
    sf::Vector2f beeVelocity = { 100.0f + float(100 * rand() % 3) , 0.0f };

    // ����
    sf::Vector2f cloudDir[3];
    float cloudSpeed[3];    
    for (int i = 0; i < 3; i++)
    {
        // �ӵ�
        int random = 1 + rand() % 10;
        cloudSpeed[i] = random * 100.0f;
        // ����
        float dirRandom = (float)rand() / RAND_MAX;
        if (dirRandom < 0.5f)
        {
            cloudDir[i].x = 1.0f;
            spriteCloud[i].setScale(-1.0f, 1.0f);
            spriteCloud[i].setPosition(0, textureCloud.getSize().y * i);
        }
        else
        {
            cloudDir[i].x = -1.0f;
            spriteCloud[i].setScale(1, 1);
            spriteCloud[i].setPosition(1920 + textureCloud.getSize().x, textureCloud.getSize().y * i);
        }
    }

    // ��
    sf::Vector2f beeDir = { 0.0f, 0.0f };
    float beeSpeed = 100;


    float random = (float)rand() / RAND_MAX;
    if (random < 0.5f)
    {
        beeDir.x = 1.0f;
        spriteBee[1].setScale(-1.0f, 1.0f);
        spriteBee[1].setPosition(textureBee.getSize().x , 800);
    }
    else
    {
        beeDir.x = -1.0f;
        spriteBee[1].setScale(1, 1);
        spriteBee[1].setPosition(1920 + textureBee.getSize().x, 800);
    }
    beeSpeed = rand() % 200 + 100;

    spriteBee[0].setPosition(500, 500);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time time = clock.restart(); // �ð� �ʱ�ȭ���� ������ �ð� ��ȯ
        deltaTime = time.asSeconds();

        // �̺�Ʈ ����
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // ���� �̵�
        for (int i = 0; i < 3; i++)
        {
            // ȭ�� ������ ���� ��� �� ����
            if (spriteCloud[i].getPosition().x < -500 || spriteCloud[i].getPosition().x > 1920 + 500)
            {
                float random = (float)rand() / RAND_MAX;
                if (random < 0.5f)
                {
                    cloudDir[i].x = 1.0f;
                    spriteCloud[i].setScale(-1.0f, 1.0f);
                    spriteCloud[i].setPosition(textureCloud.getSize().x, textureCloud.getSize().y * i);
                }
                else
                {
                    cloudDir[i].x = -1.0f;
                    spriteCloud[i].setScale(1, 1);
                    spriteCloud[i].setPosition(1920 + textureCloud.getSize().x, textureCloud.getSize().y * i);
                }
            }
            MoveSprite(spriteCloud[i], cloudDir[i], cloudSpeed[i]);
        }

        // ȭ�� ������ ���� ��� �� ����
        if (spriteBee[1].getPosition().x < -200 || spriteBee[1].getPosition().x > 1920 + 200)
        {
            float random = (float)rand() / RAND_MAX;
            if (random < 0.5f)
            {
                beeDir.x = 1.0f;
                spriteBee[1].setScale(-1.0f, 1.0f);
                spriteBee[1].setPosition(textureBee.getSize().x, 800);
            }
            else
            {
                beeDir.x = -1.0f;
                spriteBee[1].setScale(1, 1);
                spriteBee[1].setPosition(1920 + textureBee.getSize().x, 800);
            }
            beeSpeed = rand() % 200 + 100;
        }

        // �� �̵�
        //MoveSprite(spriteBee, beeDir, beeSpeed);

        sf::Vector2f pos = spriteBee[1].getPosition();
        float d = pos.x * 3.14f / 180;

        // �� �̵� (������ �)
        pos += beeDir * beeSpeed * deltaTime;
        pos.y = (sin(d) * 50) + 800;
        spriteBee[1].setPosition(pos);

        // �� �̵� (���� �̵�)

        sf::Vector2f beePos0 = spriteBee[0].getPosition();

        

        curTime += deltaTime;

        if (curTime >= nextActionTime)
        {
            randomX = rand() % 3 - 1;
            randomY = rand() % 3 - 1;

            nextActionTime = curTime + 3;
        }
    
        sf::Vector2f beeDir0 = { (float)randomX , (float)randomY };
        beePos0 += beeDir0 * 100.0f * deltaTime;
        spriteBee[0].setPosition(beePos0);

        /*
        sf::Vector2f pos = target.getPosition();
        pos += dir * speed * deltaTime;
        target.setPosition(pos);
        */

        // ȭ�鿡 ���
        window.clear();
        window.draw(spriteBackground);

        for (int i = 0; i < 3; i++)
        {
            window.draw(spriteCloud[i]);
        }

        window.draw(spriteTree);

        for (int i = 0; i < 2; i++)
        window.draw(spriteBee[i]);


        window.display();
    }



    return 0;
}

void MoveSprite(sf::Sprite& target, sf::Vector2f dir, float speed)
{
    sf::Vector2f pos = target.getPosition();
    pos += dir * speed * deltaTime;
    target.setPosition(pos);
}





