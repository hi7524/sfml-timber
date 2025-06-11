#include <SFML/Graphics.hpp>

int main()
{
    // â ����
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");
   

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

    sf::Sprite spriteBee;  // ��
    spriteBee.setTexture(textureBee);


    // ��ġ ����
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


        // Sprite �׸���
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