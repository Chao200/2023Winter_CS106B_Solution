RUN_TESTS_MENU_OPTION()

MENU_ORDER("SceneRendererGUI.cpp")

WINDOW_TITLE("The Adventures of Links")

TEST_ORDER("Labyrinth.cpp",
           "LabyrinthEscape.cpp",
           "DoublyLinkedLists.cpp",
           "ParticleSystem.cpp")

TEST_BARRIER("SceneRendererGUI.cpp", "ParticleSystem.cpp")
