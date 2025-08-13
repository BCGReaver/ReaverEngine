/**
 * @file EngineGUI.cpp
 * @brief Implementación de la clase EngineGUI para gestionar la interfaz gráfica con ImGui + SFML.
 *
 * @details
 * Esta clase se encarga de:
 * - Inicializar y apagar ImGui.
 * - Manejar el renderizado de la interfaz.
 * - Crear menús, inspector, consola y jerarquía de actores.
 * - Aplicar estilos visuales personalizados.
 *
 * Estilo aplicado:
 * - Nombres en lowerCamelCase para funciones y variables locales.
 * - Variables miembro con prefijo `m_` (si existieran).
 * - Código identado a 2 espacios.
 */

#include "EngineGUI.h"
#include "Window.h"
#include "ECS/Actor.h"

 /**
  * @brief Inicializa ImGui y configura el estilo por defecto (oscuro).
  * @param window Puntero compartido a la ventana principal.
  *
  * @details
  * Llama a `ImGui::SFML::Init` con la ventana SFML y luego aplica el estilo oscuro con `setupDarkGUIStyle()`.
  * También inicializa `selectedActorIndex` en 0.
  */
void
EngineGUI::init(const EngineUtilities::TSharedPointer<Window>& window) {
  // Inicializar ImGui con SFML
  ImGui::SFML::Init(*window->m_windowPtr);
  setupDarkGUIStyle();
  selectedActorIndex = 0;
}

/**
 * @brief Actualiza el estado de ImGui cada frame.
 * @param window    Puntero compartido a la ventana.
 * @param deltaTime Tiempo transcurrido desde el último frame.
 *
 * @details
 * Procesa eventos y actualiza la barra de menú.
 */
void
EngineGUI::update(const EngineUtilities::TSharedPointer<Window>& window,
  sf::Time deltaTime) {
  ImGui::SFML::Update(*window->m_windowPtr, deltaTime);
  barMenu();
}

/**
 * @brief Renderiza la interfaz de usuario.
 * @param window Puntero compartido a la ventana principal.
 */
void
EngineGUI::render(const EngineUtilities::TSharedPointer<Window>& window) {
  ImGui::SFML::Render(*window->m_windowPtr);
}

/**
 * @brief Procesa un evento de SFML para ImGui.
 * @param window Referencia a la ventana SFML.
 * @param event  Evento de SFML a procesar.
 */
void
EngineGUI::processEvent(const sf::Window& window, const sf::Event& event) {
  ImGui::SFML::ProcessEvent(window, event);
}

/**
 * @brief Aplica un estilo oscuro personalizado para ImGui.
 * @details
 * Configura paddings, bordes, alineaciones y paleta de colores oscuros para dar un look más profesional.
 */
void
EngineGUI::setupDarkGUIStyle() {
  // Configuración de estilo y colores (versión oscura)
  ImGuiStyle& style = ImGui::GetStyle();
  style.Alpha = 1.0f;
  style.DisabledAlpha = 1.0f;
  style.WindowPadding = ImVec2(12.0f, 12.0f);
  style.WindowRounding = 0.0f;
  style.WindowBorderSize = 0.0f;
  style.WindowMinSize = ImVec2(20.0f, 20.0f);
  style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
  style.WindowMenuButtonPosition = ImGuiDir_None;
  style.ChildRounding = 0.0f;
  style.ChildBorderSize = 1.0f;
  style.PopupRounding = 0.0f;
  style.PopupBorderSize = 1.0f;
  style.FramePadding = ImVec2(6.0f, 6.0f);
  style.FrameRounding = 0.0f;
  style.FrameBorderSize = 0.0f;
  style.ItemSpacing = ImVec2(12.0f, 6.0f);
  style.ItemInnerSpacing = ImVec2(6.0f, 3.0f);
  style.CellPadding = ImVec2(12.0f, 6.0f);
  style.IndentSpacing = 20.0f;
  style.ColumnsMinSpacing = 6.0f;
  style.ScrollbarSize = 12.0f;
  style.ScrollbarRounding = 0.0f;
  style.GrabMinSize = 12.0f;
  style.GrabRounding = 0.0f;
  style.TabRounding = 0.0f;
  style.TabBorderSize = 0.0f;
  style.ColorButtonPosition = ImGuiDir_Right;
  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

  style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
  style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549295425415f, 0.5529412031173706f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
  style.Colors[ImGuiCol_Separator] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
  style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
  style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
  style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
  style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
  style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
  style.Colors[ImGuiCol_Tab] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
  style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
  style.Colors[ImGuiCol_TabActive] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
  style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
  style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
  style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
  style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.2901960909366608f, 0.5960784554481506f, 1.0f);
  style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9960784316062927f, 0.4745098054409027f, 0.6980392336845398f, 1.0f);
  style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
  style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
  style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
  style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
  style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
  style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
  style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
  style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
}

/**
 * @brief Dibuja la barra de menú principal (File, Edit, View, Tools, Help).
 * @details
 * Cada menú tiene opciones de ejemplo y atajos de teclado. En "Exit" se cierra la app con `exit(1)`.
 */
void
EngineGUI::barMenu() {
  if (ImGui::BeginMainMenuBar()) {
    // Menú Archivo
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New")) {
        // Acción para nuevo archivo
      }
      if (ImGui::MenuItem("Open", "Ctrl+O")) {
        // Acción para abrir archivo
      }
      if (ImGui::MenuItem("Save", "Ctrl+S")) {
        // Acción para guardar archivo
      }
      if (ImGui::MenuItem("Save As..")) {
        // Acción para guardar como
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Exit", "Alt+F4")) {
        // Acción para salir
        exit(1);
      }
      ImGui::EndMenu();
    }

    // Menú Editar
    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
        // Acción para deshacer
      }
      if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
        // Acción para rehacer
      }
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "Ctrl+X")) {
        // Acción para cortar
      }
      if (ImGui::MenuItem("Copy", "Ctrl+C")) {
        // Acción para copiar
      }
      if (ImGui::MenuItem("Paste", "Ctrl+V")) {
        // Acción para pegar
      }
      ImGui::EndMenu();
    }

    // Menú Ver
    if (ImGui::BeginMenu("View")) {
      if (ImGui::MenuItem("Show Console")) {
        // Acción para mostrar la consola
      }
      if (ImGui::MenuItem("Show Hierarchy")) {
        // Acción para mostrar la jerarquía
      }
      ImGui::EndMenu();
    }

    // Menú Herramientas
    if (ImGui::BeginMenu("Tools")) {
      if (ImGui::MenuItem("Options")) {
        // Acción para mostrar opciones
      }
      ImGui::EndMenu();
    }

    // Menú Ayuda
    if (ImGui::BeginMenu("Help")) {
      if (ImGui::MenuItem("Documentation")) {
        // Acción para abrir documentación
      }
      if (ImGui::MenuItem("About")) {
        // Acción para mostrar información sobre el programa
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

/**
 * @brief Muestra la ventana de jerarquía de actores ("Hierarchy").
 * @param actors Vector con punteros compartidos a actores.
 *
 * @details
 * Permite filtrar por nombre, seleccionar actores y verlos en forma de árbol.
 */
void
EngineGUI::outliner(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
  ImGui::Begin("Hierarchy");
  // Barra de búsqueda
  static ImGuiTextFilter filter;
  filter.Draw("Search...", 180.0f); // Barra de búsqueda con ancho ajustable

  ImGui::Separator();

  // Recorrer y mostrar cada actor que pase el filtro de búsqueda
  for (int i = 0; i < actors.size(); ++i) {
    const auto& actor = actors[i];

    // Obtener el nombre del actor o asignar un nombre genérico
    std::string actorName = actor ? actor->getName() : "Unnamed Actor";

    // Verificar si el actor pasa el filtro de búsqueda
    if (!filter.PassFilter(actorName.c_str())) {
      continue; // Saltar actores que no coincidan con el filtro
    }

    // Si el actor es seleccionable
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (selectedActorIndex == i)
      flags |= ImGuiTreeNodeFlags_Selected;

    // Crear un nodo de árbol para cada actor
    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, "%s", actorName.c_str());

    // Selección de actor
    if (ImGui::IsItemClicked()) {
      selectedActorIndex = i;
      // Aquí puedes llamar a alguna función para mostrar los detalles del actor en otra ventana
    }

    // Mostrar nodos hijos si el nodo está abierto
    if (nodeOpen) {
      //ImGui::Text("Position: %.2f, %.2f, %.2f", actor->getPosition().x, actor->getPosition().y, actor->getPosition().z);
      ImGui::TreePop();
    }
  }

  ImGui::End();
}

/**
 * @brief Muestra la ventana de consola con mensajes clasificados por tipo.
 * @param programMessages Mapa con tipo de mensaje como clave y vector de strings como contenido.
 *
 * @details
 * - Los tipos de mensaje usan un color distinto (error=rojo, warning=amarillo, info=gris).
 * - Incluye filtro de búsqueda.
 * - Se auto-desplaza al final si estás en la última línea.
 */
void
EngineGUI::console(const std::map<ConsolErrorType, std::vector<std::string>>& programMessages) {
  ImGui::Begin("Console");
  static ImGuiTextFilter filter; // Filtro de búsqueda
  filter.Draw("Filter (\"error\", \"warning\", etc.)", 180.0f);

  ImGui::Separator();

  ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

  for (const auto& pair : programMessages) {
    // Establece color según el tipo de mensaje
    ImVec4 color;
    switch (pair.first) {
    case ConsolErrorType::ERROR:
      color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); // Rojo para errores
      break;
    case ConsolErrorType::WARNING:
      color = ImVec4(1.0f, 1.0f, 0.4f, 1.0f); // Amarillo para advertencias
      break;
    case ConsolErrorType::INFO:
    default:
      color = ImVec4(0.8f, 0.8f, 0.8f, 1.0f); // Gris para mensajes de información
      break;
    }

    for (const auto& message : pair.second) {
      if (!filter.PassFilter(message.c_str())) continue; // Filtrar mensajes según el filtro de búsqueda

      ImGui::PushStyleColor(ImGuiCol_Text, color);
      ImGui::Text("[%d] %s", pair.first, message.c_str());
      ImGui::PopStyleColor();
    }
  }

  // Desplazamiento automático al final
  if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    ImGui::SetScrollHereY(1.0f);

  ImGui::EndChild();
  ImGui::End();
}

/**
 * @brief Muestra la ventana de inspector para el actor seleccionado.
 * @param actors Vector de actores para inspeccionar/modificar.
 *
 * @details
 * Permite cambiar nombre, tag, layer y propiedades de transformación del actor seleccionado.
 */
void
EngineGUI::inspector(const std::vector<EngineUtilities::TSharedPointer<Actor>>& actors) {
  ImGui::Begin("Inspector");
  // Checkbox para Static
  bool isStatic = false;
  ImGui::Checkbox("##Static", &isStatic);
  ImGui::SameLine();

  // Input text para el nombre del objeto
  char objectName[128];
  std::string name = actors[selectedActorIndex]->getName();
  std::copy(name.begin(), name.end(), objectName);
  objectName[name.size()] = '\0';  // Asegurarse de terminar la cadena

  //ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() * 0.6f);
  ImGui::InputText("##ObjectName", objectName, IM_ARRAYSIZE(objectName));
  ImGui::SameLine();

  // Icono (este puede ser una imagen, aquí solo como ejemplo de botón)
  if (ImGui::Button("Icon")) {
    // Lógica del botón de icono aquí
  }

  // Separador horizontal
  ImGui::Separator();

  // Dropdown para Tag
  const char* tags[] = { "Untagged", "Player", "Enemy", "Environment" };
  static int currentTag = 0;
  //ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
  ImGui::Combo("Tag", &currentTag, tags, IM_ARRAYSIZE(tags));
  ImGui::SameLine();

  // Dropdown para Layer
  const char* layers[] = { "Default", "TransparentFX", "Ignore Raycast", "Water", "UI" };
  static int currentLayer = 0;
  //ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
  ImGui::Combo("Layer", &currentLayer, layers, IM_ARRAYSIZE(layers));

  ImGui::Separator();

  // Transform elements
  vec2Control("Position", actors[selectedActorIndex]->getComponent<Transform>()->getPosData());
  vec2Control("Rotation", actors[selectedActorIndex]->getComponent<Transform>()->getRotData());
  vec2Control("Scale", actors[selectedActorIndex]->getComponent<Transform>()->getScaData());

  ImGui::End();
}

/**
 * @brief Control personalizado para editar un `vec2` con botones de reseteo.
 * @param label        Texto que describe el control.
 * @param values       Puntero a los valores X,Y.
 * @param resetValues  Valor de reseteo para ambos ejes.
 * @param columnWidth  Ancho de la columna para el label.
 */
void EngineGUI::vec2Control(const std::string& label,
  float* values,
  float resetValues,
  float columnWidth) {
  ImGuiIO& io = ImGui::GetIO();
  auto boldFont = io.Fonts->Fonts[0];

  ImGui::PushID(label.c_str());

  ImGui::Columns(2);
  ImGui::SetColumnWidth(0, columnWidth);
  ImGui::Text(label.c_str());
  ImGui::NextColumn();

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

  float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
  ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

  float fullWidth = ImGui::CalcItemWidth();
  float itemWidth = fullWidth / 3.0f;

  // --- X Control ---
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
  ImGui::PushFont(boldFont);
  if (ImGui::Button("X", buttonSize)) values[0] = resetValues;
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::PushItemWidth(itemWidth);
  ImGui::DragFloat("##X", &values[0], 0.1f, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();
  ImGui::SameLine();

  // --- Y Control ---
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
  ImGui::PushFont(boldFont);
  if (ImGui::Button("Y", buttonSize)) values[1] = resetValues;
  ImGui::PopFont();
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::PushItemWidth(itemWidth);
  ImGui::DragFloat("##Y", &values[1], 0.1f, 0.0f, 0.0f, "%.2f");
  ImGui::PopItemWidth();
  ImGui::SameLine();

  ImGui::PopStyleVar();
  ImGui::Columns(1);
  ImGui::PopID();
}


void
EngineGUI::setupGreyGUIStyle() {
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;

  // Configuración de los colores basados en el estilo de Unreal Engine 5
  colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);  // Fondo de ventana
  colors[ImGuiCol_Border] = ImVec4(0.40f, 0.40f, 0.40f, 0.50f);  // Bordes
  colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);  // Fondo de cuadros
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);  // Hover sobre cuadros
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);  // Cuadro activo
  colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);  // Fondo del título
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);  // Título activo
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);  // Fondo de la barra de menú
  colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);  // Botón inactivo
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);  // Botón hover
  colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);  // Botón activo
  colors[ImGuiCol_Text] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);  // Texto principal
  colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);  // Texto deshabilitado
  colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);  // Encabezado
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);  // Encabezado hover
  colors[ImGuiCol_HeaderActive] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);  // Encabezado activo
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);  // Fondo scrollbar
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);  // Scrollbar inactivo
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Scrollbar hover
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Scrollbar activo
  colors[ImGuiCol_CheckMark] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);  // Marca de check

  // Ajustes de estilo general
  style.WindowRounding = 5.0f;   // Redondeo de bordes de ventanas
  style.FrameRounding = 5.0f;   // Redondeo de bordes de cuadros
  style.ScrollbarRounding = 5.0f;   // Redondeo de bordes de scrollbar
  style.GrabRounding = 5.0f;   // Redondeo de bordes de botones de agarrar
  style.FrameBorderSize = 1.0f;   // Grosor del borde de cuadros
  style.WindowBorderSize = 1.0f;   // Grosor del borde de ventanas
  style.PopupBorderSize = 1.0f;   // Grosor del borde de popups
}

/**
 * @brief Aplica un estilo gris inspirado en Unreal Engine 5 para ImGui.
 */
void
EngineGUI::setupGreyGUIStyle() {
  ImGuiStyle& style = ImGui::GetStyle();
  ImVec4* colors = style.Colors;
  // ... [código de colores igual que en tu implementación] ...
}

/**
 * @brief Apaga y limpia los recursos de ImGui.
 */
void
EngineGUI::destroy() {
  ImGui::SFML::Shutdown();
}
