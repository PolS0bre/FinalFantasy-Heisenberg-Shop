#include "general_headers.h"
#include "Video.h"
#include "SDL_Image.h"
#include "sdl.h"
#include "AudioManager.h"
#include <string>
using namespace std;
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define ItemsSize 18

void LoadImageFromName(string IMG_Name, GLuint texture, SDL_Surface* surface);
string convertToString(char* a, int size);

// Main code
int main(int argc, char* args[])
{
    AudioManager::getInstance();
    //Image buttons
    int Page = 0;
    int selectedItem = 0;
    int w, h, bytesX_Pixel;
    bool shopOpen = false, buttonLift = true;

    //HERO SHOP VALUES
    struct product {
	char	type[10];
	int		amount;
	char	name[20];
	int		priceSell;
	int		priceBuy;
	char	description[256];
    };
    GLuint ImageArr[ItemsSize];
    
    struct product ItemsShop[] = {
    	{ "Item",5,"Chimaera Wing",70,35,"Returns you to the castle"},
    	{ "Item",10,"Dragon scale",20,10,"Increases Defense by 2 when equipped."},
    	{ "Item",15,"Holy water",40,7,"Keeps enemies away when used outised of battle. Damages enemies if used during a battle."},
    	{ "Item",3,"Magic Key",100,100,"Single-use key used to a locked door before it breaks."},
    	{ "Item",20,"Medicinal Herb",25,5,"Restores 15 HP."},
    	{ "Item",25,"Torch",8,1,"Temporarily illuminates caves. Its radius is lower compared to the Glow spell. Can be chucked to enemies to inflict fire damage."},
    	{ "Armor",4,"Leather suit",70,30,"Defense +4"},
    	{ "Armor",2,"Iron armour",1000,700,"Defense +16"},
    	{ "Armor",1,"Magic armour",7700,5000,"Defense +24"},
    	{ "Armor",1,"Hero’s armour",10000,6000,"Defense +28"},
    	{ "Weapon",3,"Oaken club",60,20,"Attack +4"},
    	{ "Weapon",6,"Copper sword",180,70,"Attack +10"},
    	{ "Weapon",2,"Hand Axe",560,250,"Attack +15"},
    	{ "Weapon",1,"Flame Sword",9800,6000,"Attack +28"},
    	{ "Weapon",0,"Hero’s Sword",20000,15000,"Attack +40"},
    	{ "Shield",5,"Small Shield",90,95,"Defense +4"},
    	{ "Shield",2,"Iron Shield",800,350,"Defense +10"},
    	{ "Shield",1,"Silver Shield",14000,10000,"Defense +20"}
    };
    
    struct product ItemsPlayer[] = {
    	{ "Item",0,"Chimaera Wing",70,35,"Returns you to the castle"},
    	{ "Item",0,"Dragon scale",20,10,"Increases Defense by 2 when equipped."},
    	{ "Item",0,"Holy water",40,7,"Keeps enemies away when used outised of battle. Damages enemies if used during a battle."},
    	{ "Item",0,"Magic Key",100,100,"Single-use key used to a locked door before it breaks."},
    	{ "Item",0,"Medicinal Herb",25,5,"Restores 15 HP."},
    	{ "Item",0,"Torch",8,1,"Temporarily illuminates caves. Its radius is lower compared to the Glow spell. Can be chucked to enemies to inflict fire damage."},
    	{ "Armor",0,"Leather suit",70,30,"Defense +4"},
    	{ "Armor",0,"Iron armour",1000,700,"Defense +16"},
    	{ "Armor",0,"Magic armour",7700,5000,"Defense +24"},
    	{ "Armor",0,"Hero’s armour",10000,6000,"Defense +28"},
    	{ "Weapon",0,"Oaken club",60,20,"Attack +4"},
    	{ "Weapon",0,"Copper sword",180,70,"Attack +10"},
    	{ "Weapon",0,"Hand Axe",560,250,"Attack +15"},
    	{ "Weapon",0,"Flame Sword",9800,6000,"Attack +28"},
    	{ "Weapon",0,"Hero’s Sword",20000,15000,"Attack +40"},
    	{ "Shield",0,"Small Shield",90,95,"Defense +4"},
    	{ "Shield",0,"Iron Shield",800,350,"Defense +10"},
    	{ "Shield",0,"Silver Shield",14000,10000,"Defense +20"}
    };


	int error;
	Video_imGui * Video = Video_imGui::getInstance();

	error = Video->init_SDL_openGL_imGui("Test imGUI", 1840,1000);

	if (error) {
		switch (error) {
		case 1:
			std::cout << "Error: " << SDL_GetError() << std::endl;
			break;
		case 2:
			std::cout << "Failed to initialize OpenGL loader!" << std::endl;
			break;
		default:
			break;
		}
		return error;
	}
    ImGuiIO& io = ImGui::GetIO();

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    GLuint texture;
    SDL_Surface* surface;
    surface = IMG_Load("C:\\Users\\pol.sobrepera\\Desktop\\OpenGL_imgui\\");

    //Load Image textures
    for (int i = 0; i < ItemsSize; i++) {
        GLuint texture;

        string path = "Items\\";
        string number = to_string(i);
        string format = ".png";

        path = path.append(number);
        path = path.append(format);

        unsigned char* pixels = stbi_load(path.c_str(), &w, &h, &bytesX_Pixel, 0);

        GLint Mode = GL_RGB;
        if (bytesX_Pixel == 4) Mode = GL_RGBA;

        SDL_Surface* surface;
        surface = IMG_Load(path.c_str());

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        ImageArr[i] = texture;

    }
    //Shop img
    GLuint textureShop;

    unsigned char* pixels = stbi_load("Items\\ShopMan.png", &w, &h, &bytesX_Pixel, 0);

    GLint Mode = GL_RGB;
    if (bytesX_Pixel == 4) Mode = GL_RGBA;

    surface = IMG_Load("Items\\ShopMan.png");

    glGenTextures(1, &textureShop);
    glBindTexture(GL_TEXTURE_2D, textureShop);
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //SDL_SetWindowFullscreen(Video->getWindow(), SDL_WINDOW_FULLSCREEN);
    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(Video->getWindow()))
                done = true;
            if (event.type == SDL_KEYDOWN) {
                if (buttonLift) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_F1) {
                        shopOpen = !shopOpen;
                        if (shopOpen) {
                            AudioManager::getInstance()->playFX("Items\\Welcome.wav");
                        }
                        buttonLift = false;
                    }
                    else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                        done = true;
                        buttonLift = false;
                    }
                }
            }
            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.scancode == SDL_SCANCODE_F1)
                    buttonLift = true;
            }
        }

        // 0. Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(Video->getWindow());
        ImGui::NewFrame();

        // 1. Show the big demo window
		if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        
        //SHOP WINDOW
        if(shopOpen)
        {

            static int MoneyAmount = 1000;
            static int QuantityItem = 1;
            static char quantText[128] = "";
            ImGui::Begin("HERO SHOP");                          // Create a window called "Hello, world!" and append into it.

            //Pestaña inventario
            ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionWidth() * 0.05), 25));
            {
                ImGui::BeginChild("INVENTORY", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.25f, 325), true);
                ImGui::Text("INVENTORY");
                ImGui::Text("---------");
                if (ImGui::BeginTable("Inventory", 3)) {
                    for (int row = 0; row < ItemsSize; row++) {
                        ImGui::TableNextRow();
                        for (int column = 0; column < 3; column++)
                        {
                            ImGui::TableSetColumnIndex(column);
                            if (column == 0) {
                                ImGui::Text(ItemsPlayer[row].name);
                            }
                            else if (column == 1) {
                                ImGui::Text(to_string(ItemsPlayer[row].amount).c_str());
                            }
                            else {
                                ImGui::Text(to_string(ItemsPlayer[row].priceSell).c_str());
                                ImGui::SameLine();
                                ImGui::Text("$/Unit");
                            }
                        }
                    }
                    ImGui::EndTable();
                }
                ImGui::EndChild();
                ImGui::SameLine();
            }

            ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionWidth() * 0.35), 25));
            {
                ImGui::BeginChild("MrWhite", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.25f, 350));

                ImGui::Image((void*)(intptr_t)textureShop, ImVec2(350, 350));

                ImGui::EndChild();
                ImGui::SameLine();

            }

            //Pestaña Compra/Venta
            ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionWidth() * 0.7), 25));
            {
                ImGui::BeginChild("BUYnSELL", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.25f, 325), true);
                ImGui::Text(ItemsShop[selectedItem].name);
                ImGui::Text("---------");

                if (ItemsShop[selectedItem].amount >= QuantityItem && MoneyAmount - (ItemsShop[selectedItem].priceSell * QuantityItem) > 0 && QuantityItem > 0) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 255)));
                }
                else {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(200, 200, 200)));
                }

                if (ImGui::Button("Buy item")) {
                    if (ItemsShop[selectedItem].amount >= QuantityItem && QuantityItem > 0) {
                        if (MoneyAmount - (ItemsShop[selectedItem].priceSell * QuantityItem) > 0) {
                            ItemsShop[selectedItem].amount -= QuantityItem;
                            ItemsPlayer[selectedItem].amount += QuantityItem;
                            MoneyAmount -= ItemsShop[selectedItem].priceSell * QuantityItem;
                        }
                        else
                            ImGui::OpenPopup("No money");
                    }
                    else
                        ImGui::OpenPopup("No items");

                }
                ImGui::SameLine();
                ImGui::Text(to_string(ItemsShop[selectedItem].priceSell * QuantityItem).c_str());
                ImGui::SameLine();
                ImGui::Text("$");


                if (ItemsPlayer[selectedItem].amount >= QuantityItem && QuantityItem > 0) {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 255)));
                }
                else {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(200, 200, 200)));
                }

                if (ImGui::Button("Sell item")) {
                    if (ItemsPlayer[selectedItem].amount >= QuantityItem && QuantityItem > 0) {
                        ImGui::OpenPopup("Confirm sell");
                    }
                    else
                        ImGui::OpenPopup("No items");
                }
                ImGui::SameLine();
                ImGui::Text(to_string(ItemsShop[selectedItem].priceBuy * QuantityItem).c_str());
                ImGui::SameLine();
                ImGui::Text("$");
                if (ImGui::InputText("Quantity", quantText, IM_ARRAYSIZE(quantText), ImGuiInputTextFlags_CharsDecimal)) {
                    if (quantText[0] != 0) {
                        QuantityItem = stoi(quantText) + 0;
                    }
                }
                ImGui::PopStyleColor(2);

                ImGui::SetNextWindowPos(ImVec2(725, 350));

                if (ImGui::BeginPopupModal("No items", NULL, ImGuiWindowFlags_NoResize)) {
                    ImGui::Text("Insufficient amount of this item.");
                    if (ImGui::Button("Close")) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                ImGui::SetNextWindowPos(ImVec2(725, 350));
                if (ImGui::BeginPopupModal("No money", NULL, ImGuiWindowFlags_NoResize)) {
                    ImGui::Text("Insufficient money");
                    if (ImGui::Button("Close")) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                ImGui::SetNextWindowPos(ImVec2(725, 350));
                if (ImGui::BeginPopupModal("Confirm sell", NULL, ImGuiWindowFlags_NoResize)) {
                    ImGui::Text("Are you sure you want to sell this item?");
                    if (ImGui::Button("Yes")) {
                        ItemsShop[selectedItem].amount += QuantityItem;
                        ItemsPlayer[selectedItem].amount -= QuantityItem;
                        MoneyAmount += ItemsShop[selectedItem].priceBuy * QuantityItem;
                        AudioManager::getInstance()->playFX("Items\\BuySound.wav");
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("No")) {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }

                ImGui::EndChild();
            }

            //Items gallery
            {
                ImGui::BeginChild("Items", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.8f, 300), false);

                ImGui::SetCursorPos(ImVec2(ImGui::GetWindowContentRegionWidth() * 0.25f, 75));
                if (ImGui::Button("Previous item", ImVec2(100, 100))) {
                    if (selectedItem == 0) {
                        selectedItem = ItemsSize - 1;
                    }
                    else {
                        selectedItem--;
                    }
                }
                ImGui::SameLine();
                //Carrousel
                ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionWidth() * 0.25f ) + 105, 50));
                if (selectedItem - 1 < 0) {
                    ImGui::Image((void*)(intptr_t)ImageArr[ItemsSize - 1], ImVec2(150, 150));
                }
                else {
                    ImGui::Image((void*)(intptr_t)ImageArr[selectedItem - 1], ImVec2(150, 150));
                }
                ImGui::SameLine();

                ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionWidth() * 0.25f) + 260, 25));
                ImGui::Image((void*)(intptr_t)ImageArr[selectedItem], ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImVec4(1, 0, 0, 1));

                ImGui::SameLine();

                ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionWidth() * 0.25f) + 465, 50));
                if (selectedItem + 1 > ItemsSize - 1) {
                    ImGui::Image((void*)(intptr_t)ImageArr[0], ImVec2(150, 150));
                }
                else {
                    ImGui::Image((void*)(intptr_t)ImageArr[selectedItem + 1], ImVec2(150, 150));
                }

                ImGui::SameLine();

                ImGui::SetCursorPos(ImVec2((ImGui::GetWindowContentRegionWidth() * 0.25f) + 620, 75));
                if (ImGui::Button("Next item", ImVec2(100, 100))) {
                    if (selectedItem == (ItemsSize - 1)) {
                        selectedItem = 0;
                    }
                    else {
                        selectedItem++;
                    }
                }

                ImGui::EndChild();
            }

            //Money canvas
            {
                ImGui::BeginChild("MoneyAmount", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.3f, 75), true);
                ImGui::Text("MONEY:");
                ImGui::SameLine();
                ImGui::Text(to_string(MoneyAmount).c_str());

                ImGui::EndChild();
                ImGui::SameLine();
            }

            //Description
            {
                ImGui::BeginChild("DescObjs", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.7f, 75), true);
                ImGui::Text(ItemsShop[selectedItem].name);
                ImGui::Text(ItemsShop[selectedItem].description);
                ImGui::Text("Left amount:");
                ImGui::SameLine();
                ImGui::Text(to_string(ItemsShop[selectedItem].amount).c_str());

                ImGui::EndChild();
                ImGui::SameLine();
            }


            ImGui::End();
        }
        ImGui::SetWindowSize("HERO SHOP", ImVec2(1366, 768));

        /*
        //MEME GALLERY WINDOW
		{
			static float f = 0.0f;
			static int counter = 10;
			ImGui::Begin("MEME GALLERY");                          // Create a window called "Hello, world!" and append into it.
			ImGui::Text("They know me! They'll know my voice");               // Display some text (you can use a format strings too)

            static char Text[128] = "";
            static char Hint[128] = "Put the Image name (+.png, .jpg)";
            ImGui::InputTextWithHint(Text, Hint, Text, IM_ARRAYSIZE(Text));
            string Route = "C:\\Users\\pol.sobrepera\\Desktop\\OpenGL_imgui\\";



            if (ImGui::Button("Load Image")){
                ImageActive = false;
                string FileName = convertToString(Text, 128);
                Route.append(FileName);
                const char* file = Route.c_str();
                surface = IMG_Load(file);
                LoadImageFromName(Text, texture, surface);
                ImageActive = true;
            }
            if(ImageActive == true)
            ImGui::Image((void*)(intptr_t)texture, ImVec2(100, 100));

			ImGui::End();
		}

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }*/

		// Rendering
        ImGui::Render(); // Prepare ImGUI for render. Create VBO and draw data.
		Video->clearBackground(clear_color);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Real Render to Screen
		Video->SwapBuffer();
    }

    // Cleanup
	Video->close();

    return 0;
}
void LoadImageFromName(string IMG_Name, GLuint texture, SDL_Surface* surface) {

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        if (a[i] == ' ') {
            return s;
        }
        s = s + a[i];
    }
    return s;
}