'''
This example takes an input from the slider and outputs a different figure.

The @callback segment defines what the input and outputs are


'''

from dash import Dash, dcc, html, Input, Output, callback
import plotly.express as px

import pandas as pd

# we read in data once and only once to save memory
# expensive operations should be done "globally"
df = pd.read_csv('https://raw.githubusercontent.com/plotly/datasets/master/gapminderDataFiveYear.csv')

app = Dash()

app.layout = html.Div([
    dcc.Graph(id='graph-with-slider'),
    dcc.Slider(
        df['year'].min(),
        df['year'].max(),
        step=None,
        value=df['year'].min(),
        marks={str(year): str(year) for year in df['year'].unique()},
        id='year-slider'
    )
])

# callback function should not modify global data, 
# or else instances will be dependent on each other
@callback(  # can have multiple inputs and outputs too. Everytime one is updated, all states will be passed to function
    Output('graph-with-slider', 'figure'),  # 'figure' property of graph-with-slider is the output
    Input('year-slider', 'value'))  # 'value' property of year-slider is the input
# callback function, called whenever an update is detected
def update_figure(selected_year):
    filtered_df = df[df.year == selected_year]

    fig = px.scatter(filtered_df, x="gdpPercap", y="lifeExp",
                     size="pop", color="continent", hover_name="country",
                     log_x=True, size_max=55)

    fig.update_layout(transition_duration=500)

    return fig  # directly returns a figure to display


if __name__ == '__main__':
    app.run(debug=True)
