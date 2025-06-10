#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");
   
    sf::Texture textureBackground; // ���ȭ�� �ؽ���
    textureBackground.loadFromFile("graphics/background.png"); // ���ȭ�� �ؽ����� ���ҽ� ��� ����

    sf::Sprite spriteBackground; // �׸���� ��ü
    spriteBackground.setTexture(textureBackground); // �׸���� ��ü�� �ؽ��� ����


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(spriteBackground); // ���ȭ�� �׸���
        window.display();
    }


    return 0;
}