import math
import json


def angle_with_x_axis(point1, point2):
    delta_x = point2['x'] - point1['x']
    delta_y = point2['y'] - point1['y']

    angle_rad = math.atan2(delta_y, delta_x)
    angle_deg = math.degrees(angle_rad)

    if angle_deg < 0:
        angle_deg += 360

    return angle_deg

# Открываем файл для записи в формате JSON

def OnlineGesture(keypoints):
    # dataDegrees = []
    indx = 0
    diapozon = 50
    # with open('datash.json', 'r') as file:
    #     sh = json.load(file)
    
    with open('data_rock2.json', 'r') as file:
        rock = json.load(file)

    for i in range(len(keypoints)):
        for j in range(i + 1, len(keypoints)):
            angle = angle_with_x_axis(keypoints[i], keypoints[j])
            # dataDegrees.append({
            #     'id1': keypoints[i]['id'],
            #     'id2': keypoints[j]['id'],
            #     'angle': angle,
            # })
            # value_diapozon_sh = (sh[indx]['angle'] / 100 * diapozon)
            # max_value_sh = value_diapozon_sh + sh[indx]['angle']
            # min_value_sh = sh[indx]['angle'] - value_diapozon_sh

            value_diapozon_rock = (rock[indx]['angle'] / 100 * diapozon)
            max_value_rock = value_diapozon_rock + rock[indx]['angle']
            min_value_rock = rock[indx]['angle'] - value_diapozon_rock

            if ((angle < max_value_rock or angle > min_value_rock)):
                return False
            indx += 1

    return True