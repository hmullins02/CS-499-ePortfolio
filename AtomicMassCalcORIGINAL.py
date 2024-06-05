print("Hello, I'm here to tell you the atomic mass of elements. Go ahead and add together up to three elements and I'll tell you the total atomic mass. Please use the abbreviated version of the elements, and keep the number of each element under 20. If you only want to add together two elements, please write pass when you are asked for a third element. You can exit when asked for an element by typing exit.")

masses = [1.008, 4.003, 6.941, 9.012, 10.811, 12.011, 14.007, 15.999, 18.998, 20.180, 22.990, 24.305, 26.982, 28.086, 30.974, 32.066, 35.453, 39.948, 39.098, 40.078, 44.956, 47.867, 50.942, 51.996, 54.938, 55.845, 58.922, 58.693, 63.546, 65.380, 69.723, 72.631, 74.922, 78.971, 79.904, 83.798, 85.468, 87.620, 88.906, 91.224, 92.906, 95.950, 98.907, 101.070, 102.906, 106.420, 107.868, 112.414, 114.818, 118.711, 121.760, 127.600, 126.904, 131.293, 132.905, 137.328, 178.490, 180.948, 183.840, 186.207, 190.230, 192.217, 195.085, 196.967, 200.592, 204.383, 207.200, 208.980, 208.982, 209.987, 222.018, 223.020, 226.025, 261.000, 262.000, 266.000, 264.000, 269.000, 278.000, 281.000, 280.000, 285.000, 286.000, 289.000, 289.000, 293.000, 294.000, 294.000, 138.905, 140.116, 140.908, 144.243, 144.913, 150.360, 151.964, 157.250, 158.925, 162.500, 164.930, 167.259, 168.934, 173.055, 174.967, 227.028, 232.038, 231.036, 238.029, 237.048, 244.064, 243.061, 247.070, 247.070, 251.080, 254.000, 257.095, 258.100, 259.101, 262.000]

names = ["H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "U", "Xe", "Cs", "Ba", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Ti", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fi", "Mc", "Lv", "Ts", "Og", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr"]


#Asks for element1 and only accepts items from list
element1= (input("What's your first element? "))
while element1 not in names:
    #Let's you exit
    if element1 != "exit":
        print("that's not an element")
        element1= (input("What's your first element? "))
    else:
        exit()
#Asks for amount of element1, doesn't allow over 20
num1 = (int(input("How many molecules of this element do you want to add? ")))
while num1 > 20 or num1 <0:
    print("that's an invalid amount. give number between 0-20")
    num1 = (int(input("How many molecules of this element do you want to add? ")))


#Considering the amount of element1 is under 20, asks for a second element
if num1<=20:
    element2= (input("What's your second element? "))
    while element2 not in names:
        #Let's you exit
        if element2 != "exit":
            print("that's not an element")
            element2= (input("What's your second element? "))
        else:
            exit()

#Number of second element has to be under 20 if you want to continue
num2 = (int(input("How many molecules of this element do you want to add? ")))
while num2 > 20 or num2 <0:
    print("that's an invalid amount. give number between 0-20")
    num2 = (int(input("How many molecules of this element do you want to add? ")))



element3= (input("What's your third element? "))


idx1 = names.index(element1)
mass1 = ((masses[idx1]) * num1)

idx2 = names.index(element2)
mass2 = ((masses[idx2]) * num2)

#This is if you do want to add a third element
if element3 in names:
    num3 = (int(input("How many molecules of this element do you want to add? ")))
    while num3 > 20 or num3 <0:
        print("that's an invalid amount. give number between 0-20")
        num3 = (int(input("How many molecules of this element do you want to add? ")))


    idx3 = names.index(element3)
    mass3 = ((masses[idx3]) * num3)
    totalMass = mass1 + mass2 + mass3
    print("--------------------------------------------")
    print("Adding",num1, element1, ",", num2, element2, ", and", num3, element3, "...")
    print("......")
    print(".....")
    print("....")
    print("...")
    print("..")
    print("The total atomic mass is: ", (format(totalMass, '.3f')))
    print("--------------------------------------------")
    exit()

#This is if you only want two element added, and write pass for the third one
if element3 == "pass":
    num3 = 1
    totalMass = mass1 + mass2
    print("--------------------------------------------")
    print("Adding",num1, element1, "and", num2, element2, "...")
    print("......")
    print(".....")
    print("....")
    print("...")
    print("..")
    print("The total atomic mass is: ", (format(totalMass, '.3f')))
    print("--------------------------------------------")
    exit()

#Let's you exit when asked for a third element
if num2 <=20:
    while element3 != "pass" and element3 not in names:
        if element3 !="exit":
            print("that's not an element")
            element3= (input("What's your third element? "))
        else:
            exit()

