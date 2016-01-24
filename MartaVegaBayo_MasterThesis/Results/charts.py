# -*- coding: utf-8 -*-
"""
This module contains functions to draw diferent types of charts. This functions
are used to plot the results of the analysis.

Created on Fri May 02 10:00:09 2014
@author: Marta Vega
"""
#==============================================================================
# IMPORT MODULES AND CODE 
#==============================================================================
#Import python libraries
import numpy as np
import matplotlib.pyplot as plt

#==============================================================================
#FUNCTIONS TO DRAW GRAPHS
#
#FUNCTIONS:
#       bar_chart()
#       multiple_bar_chart()
#       histogram()
#       line_chart()
#       multiple_line_chart()
#
#==============================================================================
def bar_chart(values, xticks, title, xlabel, ylabel, barColor='b', barAlpha=1):
    """Funtion to draw a bar chart.
    @param values: values of the bars
    @param xticks: label of each bar
    @param title: title of the chart
    @param xlabel: label of the x axis
    @param ylabel: label of the y axis
    @param barColor: color of the bar
    @param barAlpha: transparency of the bars
    """
    if len(values)!=len(xticks):
        print 'Error: debe haber tantos grupos como etiquetas de barras.'
        return
    #Draw graph
    ind=np.arange(len(values))
    width=0.5
    p1 = plt.bar(ind, values, width, color=barColor, alpha=barAlpha)
    #Draw labels and titles
    plt.ylabel(ylabel)
    plt.xlabel(xlabel)
    plt.title(title)
    plt.xticks(ind+width/2., xticks ,rotation='horizontal')
    #Show
    plt.show()
    return p1
    
    
def histogram(values, title, xlabel, ylabel, n=30, barAlpha=1, barColor='b', yLog=False):
    """Function to draw a histogram.
    @param values: data whose histodram is going to be draw
    @param title: title of the graph
    @param xlabel: title of the x axis
    @param ylabel: title of the y axis
    @param n: number of bars of the histogram
    @param barAlpha: transparency of the bars
    @param barColor: color of the bars
    """
    # the histogram of the data
    n, bins, patches = plt.hist(values,n, facecolor=barColor, alpha=barAlpha , log=yLog)
    #Draw labels and titles
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    # Tweak spacing to prevent clipping of ylabel
    #plt.subplots_adjust(left=0.15)
    plt.show()
    return patches
    
def line_chart(x, y, title, xlabel, ylabel, lineColor='b', legendLabel='', lineType='o'):
    """Function to draw a line chart.
    @param x: array with the x coordinate of the points to draw
    @param y: array with the y coordinate of the points to draw
    @param xlabel: title of the x axis
    @param ylabel: title of the y axis
    @param lineColor: color of the line
    @param legendLabel: label of the line at the legend
    """
    if len(y)!=len(x):
        print 'Error: longitudes invalidas.'
        return
    plt.plot(x, y, lineType ,color=lineColor, label=legendLabel)
    #Draw labels and titles
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    
def multiple_line_chart(x_list, y_list, title, xlabel, ylabel, labels, lineColors=['b','g', 'y', 'r'], lineTypes=['o', 'o', 'o', 'o']):
    """Function to draw multiple lines in a single chart.
    @param x_list: list with and array per line with the x coordinates of the
    points to draw
    @param y_list: list with and array per line with the y coordinates of the
    points to draw
    @param title: title of the graph
    @param xlabel: title of the x axis
    @param ylabel: title of the y axis
    @param labels: labels of each line
    @param lineColors: color of each line
    @param lineTypes: type of line to draw (line, dots, stars, etc)
    """
    n=len(y_list)
    r_n=range(n)
    for i in r_n:   
        line_chart(x_list[i], y_list[i], title, xlabel, ylabel, lineColor=lineColors[i], legendLabel=labels[i], lineType=lineTypes[i])
    plt.legend()
    
