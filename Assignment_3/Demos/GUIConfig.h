RUN_TESTS_MENU_OPTION()

WINDOW_TITLE("Recursion!")

MENU_ORDER("TowersOfHanoiGUI.cpp",
           "HumanPyramidsGUI.cpp",
           "GeneticsGUI.cpp",
           "InverseGeneticsGUI.cpp")

TEST_ORDER("HumanPyramids.cpp",
           "ProteinSynthesis.cpp",
           "InverseGenetics.cpp")

TEST_BARRIER("HumanPyramidsGUI.cpp",   "HumanPyramids.cpp")
TEST_BARRIER("GeneticsGUI.cpp",        "ProteinSynthesis.cpp")
TEST_BARRIER("InverseGeneticsGUI.cpp", "InverseGenetics.cpp")
