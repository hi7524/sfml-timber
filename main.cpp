#include <SFML/Graphics.hpp>

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


    // 위치 설정
    spriteTree.setOrigin(textureTree.getSize().x * 0.5f, 0.0f);
    spriteTree.setPosition(1920 * 0.5f, 0.0f);

    for (int i = 0; i < 3; i++)
    {
        spriteCloud[i].setPosition(0.0f, textureCloud.getSize().y * i);
    }

    spriteBee.setPosition(30.0f, 800.0f);
    // spriteTree.setPosition(1920 * 0.5f - textureTree.getSize().x * 0.5f, 0);
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();


        // Sprite 그리기
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