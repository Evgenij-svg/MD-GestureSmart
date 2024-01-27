import json
import os

def remove_letters_at_end(word):

    wordNoJson = ''
    if word.endswith(".json"):
        cleaned_word = word[:-5]
        wordNoJson = cleaned_word
    else:
        wordNoJson = word


    index = len(wordNoJson)
    for i in range(len(wordNoJson) - 1, -1, -1):
        if not wordNoJson[i].isdigit():
            index = i + 1
            break
    
    # Удаляем буквы после последней цифры
    cleaned_word = word[:index]
    return cleaned_word

def Conditon_EditY(point1, point2 , indx1 , indx2):
    delta_y = point2['y'] - point1['y']

    condition = ''

    if(delta_y > 0):
        condition += f"and keypoints[{indx2}]['y'] >  keypoints[{indx1}]['y']"
    else:
        condition += f"and keypoints[{indx2}]['y'] <  keypoints[{indx1}]['y']"

    return condition

def Conditon_EditX(point1, point2 , indx1 , indx2):
    delta_y = point2['x'] - point1['x']

    condition = ''

    if(delta_y > 0):
        condition += f"and keypoints[{indx2}]['x'] >  keypoints[{indx1}]['x']"
    else:
        condition += f"and keypoints[{indx2}]['x'] <  keypoints[{indx1}]['x']"

    return condition

def check_condition(keypoint, global_condition,Json_file):
    # Загрузка данных из JSON


    # Переменная для хранения результата проверки
    result = True

    # Проверка каждого набора точек из JSON на соответствие условию
        # Указываем keypoints в качестве глобального контекста для eval
    if not eval(global_condition, {"keypoints": keypoint}):
        result = False


    if result:
        return(remove_letters_at_end(Json_file))

def find_matching_gesture(keypoint):

    
    Json_folder = './json'

    Json_files = os.listdir(Json_folder)

    for Json_file in Json_files:
        with open('./json/'+Json_file, 'r') as file:
            my_array = json.load(file)

        Global_conditon = ''
        # for i in range(len(my_array)):
        #     for j in range(i + 1, len(my_array)):
        #         Global_conditon += Conditon_Edit(my_array[i], my_array[j], i ,j)
        index= 0

        while index < len(my_array) -1 :
            Global_conditon += Conditon_EditY(my_array[index], my_array[index+1], index ,index + 1)
            # print(my_array[index])
            index += 1

        index= 1

        while index < len(my_array) - 4 :
            Global_conditon += Conditon_EditX(my_array[index], my_array[index+4], index ,index + 4)
            # print(my_array[index])
            index += 1

        Global_conditon = Global_conditon.lstrip('and ')

        result = check_condition(keypoint,Global_conditon,Json_file)
        if result:
            return(result )






# json_files_to_check = ['victory2.json', 'hand1.json', 'victory1.json','victory3.json','victory4.json','victory5.json','victory6.json',"fiksik2.json","rock2.json"]  # Замените на ваши JSON файлы
# for json_file in json_files_to_check:
#     check_condition(json_file, Global_conditon)