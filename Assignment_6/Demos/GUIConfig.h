RUN_TESTS_MENU_OPTION()

MENU_ORDER("InteractiveGUI.cpp",
           "PerformanceGUI.cpp")

TEST_ORDER("LinearProbingHashTable.cpp")

TEST_BARRIER("PerformanceGUI.cpp", "LinearProbingHashTable.cpp")

WINDOW_TITLE("The Great Stanford Hash-Off")
