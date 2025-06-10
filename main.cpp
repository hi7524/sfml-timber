#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!");
   
    sf::Texture textureBackground; // 배경화면 텍스쳐
    textureBackground.loadFromFile("graphics/background.png"); // 배경화면 텍스쳐의 리소스 경로 설정

    sf::Sprite spriteBackground; // 그리기용 객체
    spriteBackground.setTexture(textureBackground); // 그리기용 객체의 텍스쳐 설정


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(spriteBackground); // 배경화면 그리기
        window.display();
    }


    return 0;
}