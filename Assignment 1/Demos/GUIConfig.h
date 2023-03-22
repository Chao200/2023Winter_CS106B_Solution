WINDOW_TITLE("Welcome to C++!")

RUN_TESTS_MENU_OPTION()
MENU_ORDER("CallStackStorytellingGUI.cpp",
           "StackOverflowGUI.cpp",
           "FireGUI.cpp",
           "OnlyConnectGUI.cpp")
           
TEST_ORDER("PredictivePolicing.cpp",
           "Fire.cpp",
           "OnlyConnect.cpp")

TEST_BARRIER("FireGUI.cpp",        "Fire.cpp")
TEST_BARRIER("OnlyConnectGUI.cpp", "OnlyConnect.cpp")
