import re
import colorsys
import PySimpleGUI as sg

sg.theme('DarkGreen')

tab1_layout =  [
    [sg.Text('CMYK', font=('Helvetica', 24)), 
     sg.Text('C', font=('Helvetica', 24)), sg.InputText(key='T_CMYK_C', enable_events=True, default_text='0', font=('Helvetica', 24)), 
     sg.Text('M', font=('Helvetica', 24)), sg.InputText(key='T_CMYK_M', enable_events=True, default_text='0', font=('Helvetica', 24)), 
     sg.Text('Y', font=('Helvetica', 24)), sg.InputText(key='T_CMYK_Y', enable_events=True, default_text='0', font=('Helvetica', 24)), 
     sg.Text('K', font=('Helvetica', 24)), sg.InputText(key='T_CMYK_K', enable_events=True, default_text='0', font=('Helvetica', 24))],
    [sg.Text('RGB', font=('Helvetica', 24)), 
     sg.Text('R', font=('Helvetica', 24)), sg.InputText(key='T_RGB_R', enable_events=True, default_text='0', font=('Helvetica', 24)), 
     sg.Text('G', font=('Helvetica', 24)), sg.InputText(key='T_RGB_G', enable_events=True, default_text='0', font=('Helvetica', 24)), 
     sg.Text('B', font=('Helvetica', 24)), sg.InputText(key='T_RGB_B', enable_events=True, default_text='0', font=('Helvetica', 24))],
    [sg.Text('HSV', font=('Helvetica', 24)), 
     sg.Text('H', font=('Helvetica', 24)), sg.InputText(key='T_HSV_H', enable_events=True, default_text='0', font=('Helvetica', 24)), 
     sg.Text('S', font=('Helvetica', 24)), sg.InputText(key='T_HSV_S', enable_events=True, default_text='0', font=('Helvetica', 24)), 
     sg.Text('V', font=('Helvetica', 24)), sg.InputText(key='T_HSV_V', enable_events=True, default_text='0', font=('Helvetica', 24))]
]

tab2_layout = [
    [sg.Text('CMYK', font=('Helvetica', 24)), 
     sg.Text('C', font=('Helvetica', 24)), sg.Slider(range=(0.0, 100.0), tick_interval=0.1, orientation="horizontal", key='SL_CMYK_C', enable_events=True, font=('Helvetica', 24)), 
     sg.Text('M', font=('Helvetica', 24)), sg.Slider(range=(0.0, 100.0), orientation="horizontal", key='SL_CMYK_M', enable_events=True, font=('Helvetica', 24)), 
     sg.Text('Y', font=('Helvetica', 24)), sg.Slider(range=(0.0, 100.0), orientation="horizontal", key='SL_CMYK_Y', enable_events=True, font=('Helvetica', 24)), 
     sg.Text('K', font=('Helvetica', 24)), sg.Slider(range=(0.0, 100.0), orientation="horizontal", key='SL_CMYK_K', enable_events=True, font=('Helvetica', 24))],
    [sg.Text('RGB', font=('Helvetica', 24)), 
     sg.Text('R', font=('Helvetica', 24)), sg.Slider(range=(0, 255), orientation="horizontal", key='SL_RGB_R', enable_events=True, font=('Helvetica', 24)), 
     sg.Text('G', font=('Helvetica', 24)), sg.Slider(range=(0, 255), orientation="horizontal", key='SL_RGB_G', enable_events=True, font=('Helvetica', 24)), 
     sg.Text('B', font=('Helvetica', 24)), sg.Slider(range=(0, 255), orientation="horizontal", key='SL_RGB_B', enable_events=True, font=('Helvetica', 24))],
    [sg.Text('HSV', font=('Helvetica', 24)), 
     sg.Text('H', font=('Helvetica', 24)), sg.Slider(range=(0.0, 360.0), orientation="horizontal", key='SL_HSV_H', enable_events=True, font=('Helvetica', 24)), 
     sg.Text('S', font=('Helvetica', 24)), sg.Slider(range=(0, 100), orientation="horizontal", key='SL_HSV_S', enable_events=True, font=('Helvetica', 24)), 
     sg.Text('V', font=('Helvetica', 24)), sg.Slider(range=(0, 100), orientation="horizontal", key='SL_HSV_V', enable_events=True, font=('Helvetica', 24))] 
]

tab3_layout = [
    [sg.Input(text_color='white', enable_events=True, disabled = True, key='-IN-', font=('Helvetica', 24)), 
     sg.ColorChooserButton("Choose Color", font=('Helvetica', 24))]
]

Output_layout = [
    [sg.Text('CMYK: ', font=('Helvetica', 24)), sg.Text(key='CMYK', font=('Helvetica', 24))],
    [sg.Text('RGB: ', font=('Helvetica', 24)), sg.Text(key='RGB', font=('Helvetica', 24))],
    [sg.Text('HSV: ', font=('Helvetica', 24)), sg.Text(key='HSV', font=('Helvetica', 24))],
    [sg.Text('Palette: ', font=('Helvetica', 24)), sg.Text(key='PAlette', font=('Helvetica', 24))]
]

layout = [
    [sg.TabGroup([[
        sg.Tab('Slider', tab2_layout), 
        sg.Tab('Text', tab1_layout), 
        sg.Tab('Palette', tab3_layout)
    ]], font=('Helvetica', 36))],
    [sg.Graph(canvas_size=(600, 300), graph_bottom_left=(0, 0), graph_top_right=(600, 400), key="graph"), Output_layout]
]

bottomX = 10
bottomY = 50
upperX = 450
upperY = 400

def RGBSlider(window):
    r = float(values['SL_RGB_R']) / 2.55
    g = float(values['SL_RGB_G']) / 2.55
    b = float(values['SL_RGB_B']) / 2.55
    c_max = max(r, g, b)
    c_min = min(r, g, b)
    d = c_max - c_min

    window['T_RGB_R'].update(r * 2.55)
    window['T_RGB_G'].update(g * 2.55)
    window['T_RGB_B'].update(b * 2.55)
    window['RGB'].update('R: ' + str(int(r * 2.55)) + 
                         '; G: ' + str(int(g * 2.55)) + 
                         '; B: ' + str(int(b * 2.55)))

    if (max(r, g, b) != 0):
        window['SL_CMYK_K'].update(float(100 - max(r, g, b)))
        window['SL_CMYK_C'].update(float((c_max - r) / c_max) * 100)
        window['SL_CMYK_M'].update(float((c_max - g) / c_max) * 100)
        window['SL_CMYK_Y'].update(float((c_max - b) / c_max) * 100)
        window['T_CMYK_K'].update(float(100 - c_max))
        window['T_CMYK_C'].update(float((c_max - r) / c_max) * 100)
        window['T_CMYK_M'].update(float((c_max - g) / c_max) * 100)
        window['T_CMYK_Y'].update(float((c_max - b) / c_max) * 100)
        window['CMYK'].update('C: ' + str(int(((c_max - r) / c_max) * 100)) + 
                              '; M: ' + str(int(((c_max - g) / c_max) * 100)) + 
                              '; Y: ' + str(int(((c_max - b) / c_max) * 100)) + 
                              '; K: ' + str(int(100 - c_max)))
        text_color = '#'
        if len(format(int(values['SL_RGB_R']), 'X')) < 2:
            text_color = text_color + '0' + format(int(values['SL_RGB_R']), 'X')
        else:
            text_color += format(int(values['SL_RGB_R']), 'X')
        if len(format(int(values['SL_RGB_G']), 'X')) < 2:
            text_color = text_color + '0' + format(int(values['SL_RGB_G']), 'X')
        else:
            text_color += format(int(values['SL_RGB_G']), 'X')
        if len(format(int(values['SL_RGB_B']), 'X')) < 2:
            text_color = text_color + '0' + format(int(values['SL_RGB_B']), 'X')
        else:
            text_color += format(int(values['SL_RGB_B']), 'X')
    else:
        text_color = '#000000'
        window['T_CMYK_K'].update('100')
        window['SL_CMYK_K'].update('100')

    if c_max == r and d != 0 and g < b:
        h = 60 * ((g - b) / d) + 360
    elif c_max == r and d != 0 and g >= b:
        h = 60 * ((g - b) / d)
    elif c_max == g and d != 0:
        h = 60 * (((b - r) / d) + 2) % 360
    elif c_max == b and d != 0:
        h = 60 * (((r - g) / d) + 4) % 360
    else:
        h = 0
    if c_max == 0:
        s = 0
    else:
        s = 100 * d / c_max
    v = c_max

    window['T_HSV_H'].update(h)
    window['T_HSV_S'].update(s)
    window['T_HSV_V'].update(v)
    window['SL_HSV_H'].update(h)
    window['SL_HSV_S'].update(s)
    window['SL_HSV_V'].update(v)
    window['HSV'].update('H: ' + str(int(h)) + 
                         '; S: ' + str(int(s)) + 
                         '; V: ' + str(int(v)))

    window['-IN-'].update(text_color)

    window['graph'].DrawRectangle((bottomX, bottomY), (upperX, upperY), fill_color=text_color)
    
def RGBText(window):
    r = float(values['T_RGB_R']) / 2.55
    g = float(values['T_RGB_G']) / 2.55
    b = float(values['T_RGB_B']) / 2.55
    c_max = max(r, g, b)
    c_min = min(r, g, b)
    d = c_max - c_min

    window['SL_RGB_R'].update(r * 2.55)
    window['SL_RGB_G'].update(g * 2.55)
    window['SL_RGB_B'].update(b * 2.55)
    window['RGB'].update('R: ' + str(int(r * 2.55)) + 
                         '; G: ' + str(int(g * 2.55)) + 
                         '; B: ' + str(int(b * 2.55)))


    if (max(r, g, b) != 0):
        window['T_CMYK_K'].update(float(100 - max(r, g, b)))
        window['T_CMYK_C'].update(float((c_max - r) / c_max) * 100)
        window['T_CMYK_M'].update(float((c_max - g) / c_max) * 100)
        window['T_CMYK_Y'].update(float((c_max - b) / c_max) * 100)
        window['SL_CMYK_K'].update(float(100 - c_max))
        window['SL_CMYK_C'].update(float((c_max - r) / c_max) * 100)
        window['SL_CMYK_M'].update(float((c_max - g) / c_max) * 100)
        window['SL_CMYK_Y'].update(float((c_max - b) / c_max) * 100)
        window['CMYK'].update('C: ' + str(int(((c_max - r) / c_max) * 100)) + 
                              '; M: ' + str(int(((c_max - g) / c_max) * 100)) + 
                              '; Y: ' + str(int(((c_max - b) / c_max) * 100)) + 
                              '; K: ' + str(int(100 - c_max)))
        text_color = '#'
        if len(format(int(float(values['T_RGB_R'])), 'X')) < 2:
            text_color = text_color + '0' + format(int(float(values['T_RGB_R'])), 'X')
        else:
            text_color += format(int(float(values['T_RGB_R'])), 'X')
        if len(format(int(float(values['T_RGB_G'])), 'X')) < 2:
            text_color = text_color + '0' + format(int(float(values['T_RGB_G'])), 'X')
        else:
            text_color += format(int(float(values['T_RGB_G'])), 'X')
        if len(format(int(float(values['T_RGB_B'])), 'X')) < 2:
            text_color = text_color + '0' + format(int(float(values['T_RGB_B'])), 'X')
        else:
            text_color += format(int(float(values['T_RGB_B'])), 'X')
    else:
        text_color = '#000000'
        window['T_CMYK_K'].update('100')
        window['SL_CMYK_K'].update('100')

    if c_max == r and d != 0 and g < b:
        h = 60 * ((g - b) / d) + 360
    elif c_max == r and d != 0 and g >= b:
        h = 60 * ((g - b) / d)
    elif c_max == g and d != 0:
        h = 60 * (((b - r) / d) + 2) % 360
    elif c_max == b and d != 0:
        h = 60 * (((r - g) / d) + 4) % 360
    else:
        h = 0
    if c_max == 0:
        s = 0
    else:
        s = 100 * d / c_max
    v = c_max

    window['T_HSV_H'].update(h)
    window['T_HSV_S'].update(s)
    window['T_HSV_V'].update(v)
    window['SL_HSV_H'].update(h)
    window['SL_HSV_S'].update(s)
    window['SL_HSV_V'].update(v)
    window['HSV'].update('H: ' + str(int(h)) + 
                         '; S: ' + str(int(s)) + 
                         '; V: ' + str(int(v)))

    window['-IN-'].update(text_color)

    window['graph'].DrawRectangle((bottomX, bottomY), (upperX, upperY), fill_color=text_color)
    
def CMYKSlider(window):
    window['T_CMYK_C'].update(values['SL_CMYK_C'])
    window['T_CMYK_M'].update(values['SL_CMYK_M'])
    window['T_CMYK_Y'].update(values['SL_CMYK_Y'])
    window['T_CMYK_K'].update(values['SL_CMYK_K'])
    window['CMYK'].update('C: ' + str(values['SL_CMYK_C']) + 
                          '; M: ' + str(values['SL_CMYK_M']) + 
                          '; Y: ' + str(values['SL_CMYK_Y']) + 
                          '; K: ' + str(values['SL_CMYK_K']))

    r = float(0.0255 * (100 - values['SL_CMYK_C']) * (100 - values['SL_CMYK_K']))
    g = float(0.0255 * (100 - values['SL_CMYK_M']) * (100 - values['SL_CMYK_K']))
    b = float(0.0255 * (100 - values['SL_CMYK_Y']) * (100 - values['SL_CMYK_K']))
    window['RGB'].update('R: ' + str(int(r)) + 
                         '; G: ' + str(int(g)) + 
                         '; B: ' + str(int(b)))
    if abs(r - round(r)) < 0.001:
        r = round(r)
    if abs(g - round(g)) < 0.001:
        g = round(g)
    if abs(b - round(b)) < 0.001:
        b = round(b)
    c_max = max(r, g, b)
    c_min = min(r, g, b)
    d = c_max - c_min

    window['SL_RGB_R'].update(round(r, 1))
    window['SL_RGB_G'].update(round(g, 1))
    window['SL_RGB_B'].update(round(b, 1))
    window['T_RGB_R'].update(int(round(r, 1)))
    window['T_RGB_G'].update(int(round(g, 1)))
    window['T_RGB_B'].update(int(round(b, 1)))
    text_color = '#'

    if (max(r, g, b) != 0):
        if len(format(int(r), 'X')) < 2:
            text_color = text_color + '0' + format(int(r), 'X')
        else:
            text_color += format(int(r), 'X')
        if len(format(int(g), 'X')) < 2:
            text_color = text_color + '0' + format(int(g), 'X')
        else:
            text_color += format(int(g), 'X')
        if len(format(int(b), 'X')) < 2:
            text_color = text_color + '0' + format(int(b), 'X')
        else:
            text_color += format(int(b), 'X')
    else:
        text_color = '#000000'
        window['T_CMYK_K'].update('100')
        window['SL_CMYK_K'].update('100')

    if c_max == r and d != 0 and g < b:
        h = 60 * (g - b / d) % 360
    elif c_max == r and d != 0 and g >= b:
        h = 60 * (((g - b) / d) + 6) % 360
    elif c_max == g and d != 0:
        h = 60 * (((b - r) / d) + 2) % 360
    elif c_max == b and d != 0:
        h = 60 * (((r - g) / d) + 4) % 360
    else:
        h = 0
    if c_max == 0:
        s = 0
    else:
        s = 100 * d / c_max
    v = c_max / 2.55

    window['T_HSV_H'].update(int(round(h, 1)))
    window['T_HSV_S'].update(int(round(s, 1)))
    window['T_HSV_V'].update(int(round(v, 1)))
    window['SL_HSV_H'].update(h)
    window['SL_HSV_S'].update(s)
    window['SL_HSV_V'].update(v)
    window['HSV'].update('H: ' + str(int(h)) + 
                         '; S: ' + str(int(s)) + 
                         '; V: ' + str(int(v)))

    window['-IN-'].update(text_color)

    window['graph'].DrawRectangle((bottomX, bottomY), (upperX, upperY), fill_color=text_color)
    
def CMYKText(window):
    window['SL_CMYK_C'].update(int(float(values['T_CMYK_C'])))
    window['SL_CMYK_M'].update(int(float(values['T_CMYK_M'])))
    window['SL_CMYK_Y'].update(int(float(values['T_CMYK_Y'])))
    window['SL_CMYK_K'].update(int(float(values['T_CMYK_K'])))
    window['CMYK'].update('C: ' + values['T_CMYK_C'] + 
                          '; M: ' + values['T_CMYK_M'] + 
                          '; Y: ' + values['T_CMYK_Y'] + 
                          '; K: ' + values['T_CMYK_K'])

    r = float(0.0255 * (100 - int(float(values['T_CMYK_C']))) * (100 - int(float(values['T_CMYK_K']))))
    g = float(0.0255 * (100 - int(float(values['T_CMYK_M']))) * (100 - int(float(values['T_CMYK_K']))))
    b = float(0.0255 * (100 - int(float(values['T_CMYK_Y']))) * (100 - int(float(values['T_CMYK_K']))))
    window['RGB'].update('R: ' + str(int(r)) + 
                         '; G: ' + str(int(g)) + 
                         '; B: ' + str(int(b)))
    if abs(r - round(r)) < 0.001:
        r = round(r)
    if abs(g - round(g)) < 0.001:
        g = round(g)
    if abs(b - round(b)) < 0.001:
        b = round(b)
    c_max = max(r, g, b)
    c_min = min(r, g, b)
    d = c_max - c_min

    window['SL_RGB_R'].update(round(r, 1))
    window['SL_RGB_G'].update(round(g, 1))
    window['SL_RGB_B'].update(round(b, 1))
    window['T_RGB_R'].update(int(round(r, 1)))
    window['T_RGB_G'].update(int(round(g, 1)))
    window['T_RGB_B'].update(int(round(b, 1)))
    text_color = '#'

    if (max(r, g, b) != 0):
        if len(format(int(r), 'X')) < 2:
            text_color = text_color + '0' + format(int(r), 'X')
        else:
            text_color += format(int(r), 'X')
        if len(format(int(g), 'X')) < 2:
            text_color = text_color + '0' + format(int(g), 'X')
        else:
            text_color += format(int(g), 'X')
        if len(format(int(b), 'X')) < 2:
            text_color = text_color + '0' + format(int(b), 'X')
        else:
            text_color += format(int(b), 'X')
    else:
        text_color = '#000000'
        window['T_CMYK_K'].update('100')
        window['SL_CMYK_K'].update('100')

    if c_max == r and d != 0 and g < b:
        h = 60 * (g - b / d) % 360
    elif c_max == r and d != 0 and g >= b:
        h = 60 * (((g - b) / d) + 6) % 360
    elif c_max == g and d != 0:
        h = 60 * (((b - r) / d) + 2) % 360
    elif c_max == b and d != 0:
        h = 60 * (((r - g) / d) + 4) % 360
    else:
        h = 0
    if c_max == 0:
        s = 0
    else:
        s = 100 * d / c_max
    v = c_max / 2.55

    window['T_HSV_H'].update(int(round(h, 1)))
    window['T_HSV_S'].update(int(round(s, 1)))
    window['T_HSV_V'].update(int(round(v, 1)))
    window['SL_HSV_H'].update(h)
    window['SL_HSV_S'].update(s)
    window['SL_HSV_V'].update(v)
    window['HSV'].update('H: ' + str(int(h)) + 
                         '; S: ' + str(int(s)) + 
                         '; V: ' + str(int(v)))

    window['-IN-'].update(text_color)

    window['graph'].DrawRectangle((bottomX, bottomY), (upperX, upperY), fill_color=text_color)
    
def HSVSlider(window):
    window['T_HSV_H'].update(values['SL_HSV_H'])
    window['T_HSV_S'].update(values['SL_HSV_S'])
    window['T_HSV_V'].update(values['SL_HSV_V'])
    window['HSV'].update('H: ' + str(int(values['SL_HSV_H'])) + 
                         '; S: ' + str(int(values['SL_HSV_S'])) + 
                         '; V: ' + str(int(values['SL_HSV_V'])))

    h = float(values['SL_HSV_H'])
    s = float(values['SL_HSV_S'])
    v = float(values['SL_HSV_V'])

    h_ = int(h / 60) % 6
    v_min = (100 - s) * v / 100
    a = (v - v_min) * (h % 60) / 60
    v_inc = v_min + a
    v_dec = v - a
    r, g, b = 0, 0, 0
    if h_ == 0:
        r, g, b = v, v_inc, v_min
    elif h_ == 1:
        r, g, b = v_dec, v, v_min
    elif h_ == 2:
        r, g, b = v_min, v, v_inc
    elif h_ == 3:
        r, g, b = v_min, v_dec, v
    elif h_ == 4:
        r, g, b = v_inc, v_min, v
    else:
        r, g, b = v, v_min, v_dec
    r *= 2.55
    g *= 2.55
    b *= 2.55
    window['SL_RGB_R'].update(r)
    window['SL_RGB_G'].update(g)
    window['SL_RGB_B'].update(b)
    window['T_RGB_R'].update(r)
    window['T_RGB_G'].update(g)
    window['T_RGB_B'].update(b)
    window['RGB'].update('R: ' + str(int(r)) + 
                         '; G: ' + str(int(g)) + 
                         '; B: ' + str(int(b)))
    text_color = '#'

    if abs(r - round(r, 2)) < 0.001:
        r = round(round(r, 2))
    if abs(g - round(g, 2)) < 0.001:
        g = round(round(g, 2))
    if abs(b - round(b, 2)) < 0.001:
        b = round(round(b, 2))
    r /= 2.55
    g /= 2.55
    b /= 2.55
    c_max = max(r, g, b)
    c_min = min(r, g, b)
    d = c_max - c_min

    if (max(r, g, b) != 0):
        window['T_CMYK_K'].update(float(100 - max(r, g, b)))
        window['T_CMYK_C'].update(float((c_max - r) / c_max) * 100)
        window['T_CMYK_M'].update(float((c_max - g) / c_max) * 100)
        window['T_CMYK_Y'].update(float((c_max - b) / c_max) * 100)
        window['SL_CMYK_K'].update(float(100 - c_max))
        window['SL_CMYK_C'].update(float((c_max - r) / c_max) * 100)
        window['SL_CMYK_M'].update(float((c_max - g) / c_max) * 100)
        window['SL_CMYK_Y'].update(float((c_max - b) / c_max) * 100)
        window['CMYK'].update('C: ' + str(int(((c_max - r) / c_max) * 100)) + 
                              '; M: ' + str(int(((c_max - g) / c_max) * 100)) + 
                              '; Y: ' + str(int(((c_max - b) / c_max) * 100)) + 
                              '; K: ' + str(int(100 - c_max)))
        r *= 2.55
        g *= 2.55
        b *= 2.55
        if len(format(int(r), 'X')) < 2:
            text_color = text_color + '0' + format(int(r), 'X')
        else:
            text_color += format(int(r), 'X')
        if len(format(int(g), 'X')) < 2:
            text_color = text_color + '0' + format(int(g), 'X')
        else:
            text_color += format(int(g), 'X')
        if len(format(int(b), 'X')) < 2:
            text_color = text_color + '0' + format(int(b), 'X')
        else:
            text_color += format(int(b), 'X')
    else:
        text_color = '#000000'
        window['T_CMYK_K'].update('100')
        window['SL_CMYK_K'].update('100')

    window['-IN-'].update(text_color)

    window['graph'].DrawRectangle((bottomX, bottomY), (upperX, upperY), fill_color=text_color)
    
def HSVText(window):
    window['SL_HSV_H'].update(int(float(values['T_HSV_H'])))
    window['SL_HSV_S'].update(int(float(values['T_HSV_S'])))
    window['SL_HSV_V'].update(int(float(values['T_HSV_V'])))
    window['HSV'].update('H: ' + values['T_HSV_H'] + 
                         '; S: ' + values['T_HSV_S'] + 
                         '; V: ' + values['T_HSV_V'])

    h, s, v = float(values['T_HSV_H']), float(values['T_HSV_S']), float(values['T_HSV_V'])

    h_ = int(h / 60) % 6
    v_min = (100 - s) * v / 100
    a = (v - v_min) * (h % 60) / 60
    v_inc = v_min + a
    v_dec = v - a
    r, g, b = 0, 0, 0
    if h_ == 0:
        r, g, b = v, v_inc, v_min
    elif h_ == 1:
        r, g, b = v_dec, v, v_min
    elif h_ == 2:
        r, g, b = v_min, v, v_inc
    elif h_ == 3:
        r, g, b = v_min, v_dec, v
    elif h_ == 4:
        r, g, b = v_inc, v_min, v
    else:
        r, g, b = v, v_min, v_dec
    r *= 2.55
    g *= 2.55
    b *= 2.55
    window['SL_RGB_R'].update(r)
    window['SL_RGB_G'].update(g)
    window['SL_RGB_B'].update(b)
    window['T_RGB_R'].update(r)
    window['T_RGB_G'].update(g)
    window['T_RGB_B'].update(b)
    window['RGB'].update('R: ' + str(int(r)) + 
                         '; G: ' + str(int(g)) + 
                         '; B: ' + str(int(b)))
    text_color = '#'

    if abs(r - round(r)) < 0.001:
        r = round(round(r, 2))
    if abs(g - round(g)) < 0.001:
        g = round(round(g, 2))
    if abs(b - round(b)) < 0.001:
        b = round(round(b, 2))
    r /= 2.55
    g /= 2.55
    b /= 2.55
    c_max = max(r, g, b)
    c_min = min(r, g, b)
    d = c_max - c_min

    if (max(r, g, b) != 0):
        window['T_CMYK_K'].update(float(100 - max(r, g, b)))
        window['T_CMYK_C'].update(float((c_max - r) / c_max) * 100)
        window['T_CMYK_M'].update(float((c_max - g) / c_max) * 100)
        window['T_CMYK_Y'].update(float((c_max - b) / c_max) * 100)
        window['SL_CMYK_K'].update(float(100 - c_max))
        window['SL_CMYK_C'].update(float((c_max - r) / c_max) * 100)
        window['SL_CMYK_M'].update(float((c_max - g) / c_max) * 100)
        window['SL_CMYK_Y'].update(float((c_max - b) / c_max) * 100)
        window['CMYK'].update('C: ' + str(int(((c_max - r) / c_max) * 100)) + 
                              '; M: ' + str(int(((c_max - g) / c_max) * 100)) + 
                              '; Y: ' + str(int(((c_max - b) / c_max) * 100)) + 
                              '; K: ' + str(int(100 - c_max)))
        r *= 2.55
        g *= 2.55
        b *= 2.55
        if len(format(int(r), 'X')) < 2:
            text_color = text_color + '0' + format(int(r), 'X')
        else:
            text_color += format(int(r), 'X')
        if len(format(int(g), 'X')) < 2:
            text_color = text_color + '0' + format(int(g), 'X')
        else:
            text_color += format(int(g), 'X')
        if len(format(int(b), 'X')) < 2:
            text_color = text_color + '0' + format(int(b), 'X')
        else:
            text_color += format(int(b), 'X')
    else:
        text_color = '#000000'
        window['T_CMYK_K'].update('100')
        window['SL_CMYK_K'].update('100')

    window['-IN-'].update(text_color)

    window['graph'].DrawRectangle((bottomX, bottomY), (upperX, upperY), fill_color=text_color)
    
def Change(window):
    if re.match(r'#[0-9A-Fa-f]{6}', values['-IN-']):
        r = float(int(values['-IN-'][1:3], 16)) / 2.55
        g = float(int(values['-IN-'][3:5], 16)) / 2.55
        b = float(int(values['-IN-'][5:7], 16)) / 2.55
        c_max = max(r, g, b)
        c_min = min(r, g, b)
        d = c_max - c_min

        window['T_RGB_R'].update(r * 2.55)
        window['T_RGB_G'].update(g * 2.55)
        window['T_RGB_B'].update(b * 2.55)
        window['SL_RGB_R'].update(r * 2.55)
        window['SL_RGB_G'].update(g * 2.55)
        window['SL_RGB_B'].update(b * 2.55)

        if (max(r, g, b) != 0):
            window['SL_CMYK_K'].update(float(100 - max(r, g, b)))
            window['SL_CMYK_C'].update(float((c_max - r) / c_max) * 100)
            window['SL_CMYK_M'].update(float((c_max - g) / c_max) * 100)
            window['SL_CMYK_Y'].update(float((c_max - b) / c_max) * 100)
            window['T_CMYK_K'].update(float(100 - c_max))
            window['T_CMYK_C'].update(float((c_max - r) / c_max) * 100)
            window['T_CMYK_M'].update(float((c_max - g) / c_max) * 100)
            window['T_CMYK_Y'].update(float((c_max - b) / c_max) * 100)
            text_color = '#'
            if len(format(int(r * 2.55), 'X')) < 2:
                text_color = text_color + '0' + format(int(r * 2.55), 'X')
            else:
                text_color += format(int(r * 2.55), 'X')
            if len(format(int(g * 2.55), 'X')) < 2:
                text_color = text_color + '0' + format(int(g * 2.55), 'X')
            else:
                text_color += format(int(g * 2.55), 'X')
            if len(format(int(b * 2.55), 'X')) < 2:
                text_color = text_color + '0' + format(int(b * 2.55), 'X')
            else:
                text_color += format(int(b * 2.55), 'X')
        else:
            text_color = '#000000'
            window['T_CMYK_K'].update('100')
            window['SL_CMYK_K'].update('100')

        if c_max == r and d != 0 and g < b:
            h = 60 * ((g - b) / d) % 360
        elif c_max == r and d != 0 and g >= b:
            h = 60 * ((g - b) / d)
        elif c_max == g and d != 0:
            h = 60 * (((b - r) / d) + 2) % 360
        elif c_max == b and d != 0:
            h = 60 * (((r - g) / d) + 4) % 360
        else:
            h = 0
        if c_max == 0:
            s = 0
        else:
            s = 100 * d / c_max
        v = c_max

        window['T_HSV_H'].update(h)
        window['T_HSV_S'].update(s)
        window['T_HSV_V'].update(v)
        window['SL_HSV_H'].update(h)
        window['SL_HSV_S'].update(s)
        window['SL_HSV_V'].update(v)
        window['-IN-'].update(text_color)
        window['graph'].DrawRectangle((bottomX, bottomY), (upperX, upperY), fill_color=text_color)
    else:
        window['T_HSV_H'].update(0)
        window['T_HSV_S'].update(0)
        window['T_HSV_V'].update(0)
        window['SL_HSV_H'].update(0)
        window['SL_HSV_S'].update(0)
        window['SL_HSV_V'].update(0)
        window['SL_CMYK_K'].update(0)
        window['SL_CMYK_C'].update(0)
        window['SL_CMYK_M'].update(0)
        window['SL_CMYK_Y'].update(0)
        window['T_CMYK_K'].update(0)
        window['T_CMYK_C'].update(0)
        window['T_CMYK_M'].update(0)
        window['T_CMYK_Y'].update(0)
        window['T_RGB_R'].update(0)
        window['T_RGB_G'].update(0)
        window['T_RGB_B'].update(0)
        window['SL_RGB_R'].update(0)
        window['SL_RGB_G'].update(0)
        window['SL_RGB_B'].update(0)

window = sg.Window('Color Application', layout, resizable=True)
while True:
    event, values = window.read()
    text_color = '#000000'
    window['graph'].DrawRectangle((bottomX, bottomY), (upperX, upperY), fill_color=text_color)
    if event == sg.WIN_CLOSED or event == 'Cancel':
        break
    if event == '-IN-':
      window['graph'].DrawRectangle((bottomX, bottomY), (upperX, upperY), fill_color=values['-IN-'])

    # Обработка RGB слайдера
    if(event == 'SL_RGB_R' or event == 'SL_RGB_G' or event == 'SL_RGB_B'):
        RGBSlider(window)
    
    # Обработка RGB текста
    if((event == 'T_RGB_R' and values['T_RGB_R'].isnumeric() and int(values['T_RGB_R']) >= 0 and int(values['T_RGB_R']) <= 255) or 
       (event == 'T_RGB_G' and values['T_RGB_G'].isnumeric() and int(values['T_RGB_G']) >= 0 and int(values['T_RGB_G']) <= 255) or 
       (event == 'T_RGB_B' and values['T_RGB_B'].isnumeric() and int(values['T_RGB_B']) >= 0 and int(values['T_RGB_B']) <= 255)):
        RGBText(window)

    # Обработка CMYK слайдера
    if(event == 'SL_CMYK_C' or event == 'SL_CMYK_M' or event == 'SL_CMYK_Y' or event == 'SL_CMYK_K'):
        CMYKSlider(window)
        
    # Обработка CMYK текста
    if ((event == 'T_CMYK_C' and values['T_CMYK_C'].isnumeric() and int(values['T_CMYK_C']) >= 0 and int(values['T_CMYK_C']) <= 100) or 
        (event == 'T_CMYK_M' and values['T_CMYK_M'].isnumeric() and int(values['T_CMYK_M']) >= 0 and int(values['T_CMYK_M']) <= 100) or 
        (event == 'T_CMYK_Y' and values['T_CMYK_Y'].isnumeric() and int(values['T_CMYK_Y']) >= 0 and int(values['T_CMYK_Y']) <= 100) or 
        (event == 'T_CMYK_K' and values['T_CMYK_K'].isnumeric() and int(values['T_CMYK_K']) >= 0 and int(values['T_CMYK_K']) <= 100)):
        CMYKText(window)

    # Обработка HSV слайдера
    if (event == 'SL_HSV_H' or event == 'SL_HSV_S' or event == 'SL_HSV_V'):
        HSVSlider(window)

    # Обработка HSV текста
    if ((event == 'T_HSV_H' and values['T_HSV_H'].isnumeric() and int(values['T_HSV_H']) >= 0 and int(values['T_HSV_H']) <= 360) or 
        (event == 'T_HSV_S' and values['T_HSV_S'].isnumeric() and int(values['T_HSV_S']) >= 0 and int(values['T_HSV_S']) <= 100) or 
        (event == 'T_HSV_V' and values['T_HSV_V'].isnumeric() and int(values['T_HSV_V']) >= 0 and int(values['T_HSV_V']) <= 100)):
        HSVText(window)

    if event == '-IN-' and values['-IN-'] is not None:
        Change(window) 

window.close()