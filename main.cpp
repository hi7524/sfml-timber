#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

int main()
{
    // 창 설정
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");
   

    // 텍스쳐 설정
    sf::Texture textureBackground;                      
    textureBackground.loadFromFile("graphics/background.png"); 

    sf::Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    sf::Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");

    sf::Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    

    // 스프라이트
    sf::Sprite spriteBackground; // 배경        
    spriteBackground.setTexture(textureBackground);

    sf::Sprite spriteCloud[3];  // 구름
    for (int i = 0; i < 3; i++)
    {
        spriteCloud[i].setTexture(textureCloud);
    }
    
    sf::Sprite spriteTree; // 나무
    spriteTree.setTexture(textureTree);

    sf::Sprite spriteBee;  // 벌
    spriteBee.setTexture(textureBee);


    // 초기 위치 설정
    spriteTree.setOrigin(textureTree.getSize().x * 0.5f, 0.0f);
    spriteTree.setPosition(1920 * 0.5f, 0.0f);

    for (int i = 0; i < 3; i++)
    {
        spriteCloud[i].setPosition(0.0f, textureCloud.getSize().y * i);
    }

    spriteBee.setPosition(30.0f, 800.0f);
    // spriteTree.setPosition(1920 * 0.5f - textureTree.getSize().x * 0.5f, 0);


    // 초기 설정
    sf::Vector2f velocity = { 200.0f, 0.0f };

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Time time = clock.restart();    // 시간 초기화까지 누적된 시간 반환
        float deltaTime = time.asSeconds(); 

        // 이벤트 루프
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // 벌 이동
        sf::Vector2f pos = spriteBee.getPosition();
        pos += velocity * deltaTime;
        spriteBee.setPosition(pos);

        // 화면에 출력
        window.clear();
        window.draw(spriteBackground);

        for (int i = 0; i < 3; i++)
        {
            window.draw(spriteCloud[i]);
        }
        
        window.draw(spriteTree);
        
        window.draw(spriteBee);


        window.display();
    }


    return 0;
}