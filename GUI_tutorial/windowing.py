import PySimpleGUI as sg

# Tutorial associated with this file is here: 
# https://docs.pysimplegui.com/en/latest/documentation/quick_start/windowing_apis_popup_windows/#popup-windows-4-types

# Check PySimpleGUI documentation for full list of popup functions

# Popups are windows with functions defined by PySimpleGUI. 
# If a more customizable window is needed, skip this section.

'''
First popup group: output popups

1) Displays text in window
2) Suspends tasks
3) Returns user input/button click. Could be close window as well
'''

result = sg.popup('This is your basic popup', 'You can have multiple arugments')
sg.popup(f'Returned restult = {result}')

result = sg.popup_yes_no('A yes_no popup makes it easy to ask a question, right?')
sg.popup(f'Returned restult = {result}')

result = sg.popup_no_titlebar('Has no titlebar')
sg.popup(f'Returned restult = {result}')

result = sg.popup_auto_close('If you do nothing, this popup closes after 2 seconds', auto_close_duration=2)
sg.popup(f'You entered {result}')



'''
Second popup group: input popups

Takes input from user and returns a string (text, file, folder) or tuple (date).
Closing the window returns WIN_CLOSED and thus result is none.
'''

text = sg.popup_get_text("What's your name?")
sg.popup(f'You entered {text}')

filename = sg.popup_get_file('Please enter a filename')
sg.popup(f'You entered {filename}')

folder = sg.popup_get_folder('Choose a folder')
sg.popup(f'You entered {folder}')

date = sg.popup_get_date()
sg.popup(f'You entered {date}')


'''
Third popup group: debug popups

Prints strings into a window. Can modify text and highlight colors. Can add wait parameter to stop code flow.
Less commonly used than popup, even for debugging.

NOTE: not obvious to be what end does yet...
'''

sg.Print('Hello, World of GUIs!')
sg.Print('While I have your attention...', end='', colors='white on red')
sg.Print('\nsg.Print looks much like the Python ', end='')
sg.Print('print', colors='white on green', end='')
sg.Print(' statement.')
sg.Print('But it also includes FUN additions like colors', text_color='purple')
sg.Print('As well a "wait" parameter to stop the program from exiting.', wait=True)


'''
Fourth popup group: one-line progress meter

Literally a progress meter! Basically a for/while loop.
Can be aborted with cancel button in window.
''' 

for i in range(0, 500):
    # function(title, current counter, max value)
    sg.one_line_progress_meter("The most simple progress meter possible", i, 499)
    
sg.popup('Done!')



# Custom windows can be designed with instructions here.

'''
One shot windows: only appear once and will not be opened again or last longer.

Has a compact code format but I thought this provides more clarity.

Follows the layout -> window -> window.read() structure. Closes window at the end.

'''
layout = [[sg.Text('This is our one-shot window')],
          [sg.Button('Ok')]]

window = sg.Window('Title', layout)

event, values = window.read()

window.close()

'''
Persistent windows: can be interacted with over an extended period of time.

Follows the layout -> window -> window.read() structure. Closes window at the end.

Check for window close is needed since if closed "event" won't be readable.

'''

layout = [[sg.Text('This is our persistent window')],
          [sg.Button('1'), sg.Button('2'), sg.Button('Exit')]]

window = sg.Window('Title', layout)

while True:         # The Event Looop
    event, values = window.read()
    if event == sg.WIN_CLOSED or event == 'Exit':
        break

    if event == '1':
        sg.popup('You clicked 1')
    elif event == '2':
        sg.popup('You clicked 2')

window.close()