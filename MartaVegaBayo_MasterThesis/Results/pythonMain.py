"""
This program analyzes the results obtaining of the Dataset generation and of the IR systems developed.
"""
#==============================================================================
# IMPORT MODULES, DEFINE CONSTANTS AND DEFINE MAIN FUNCTION 
#==============================================================================
import charts
import networkx as nx
import numpy as np
#import scipy.stats as sp
import matplotlib.pyplot as plt
#import spm1d
from matplotlib import pylab
import MySQLdb

DB_HOST = 'localhost'
DB_USER = 'root'
DB_PASS = '1234'
DB_NAME = 'ArticleDataset'


def main():
    analyze_network()
    #analyze_parser()
    #analyze_dataset_generator()
    #analyze_IRSystems()

#==============================================================================
# 			FUNCTIONS TO DO THE ANALYSIS
#==============================================================================
def analyze_IRSystems():
    """
    This function analyses and compares the Title Based IR and the Citation Based IR. In particular, it
    analyses the execution time of the IR systems and the Reciprocal Ranks obtained by them.
    The distribution of each measure in each IR system is obtained. In addition, the Srudent's paired
    t-test is applied to compare them.
    """
    n=30
    #ANALYZE TITLE BASED IR
    #Get reciprocal rank
    f=open(".\IRtesting\IRTitleBased_RR.txt")
    lines=f.readlines()
    reciprocalRank_IRTitleBased=[float(e.strip()) for e in lines]
    reciprocalRank_IRTitleBased=np.asarray(reciprocalRank_IRTitleBased)
    #Get histogram
    title="Reciprocal Rank values distribution obtained with the TitleBased IR"
    xlabel="Reciprocal Rank"
    ylabel=""
    charts.histogram(reciprocalRank_IRTitleBased, title, xlabel, ylabel, n)
    MRR_IR1=np.mean(reciprocalRank_IRTitleBased)
    varRR_IR1=np.var(reciprocalRank_IRTitleBased, 1)
    #Get execution times
    f=open(".\IRtesting\IRTitleBased_MSEC.txt")
    lines=f.readlines()
    exeTimes_IRTitleBased=[float(e.strip()) for e in lines]
    exeTimes_IRTitleBased=np.asarray(exeTimes_IRTitleBased)
    #Get histogram
    title="Execution times distribution obtained with the TitleBased IR"
    xlabel="Execution time (msec)"
    ylabel=""
    charts.histogram(exeTimes_IRTitleBased, title, xlabel, ylabel, n)
    meanTime_IR1=np.mean(exeTimes_IRTitleBased)
    varTime_IR1=np.var(exeTimes_IRTitleBased, 1)

    #ANALYZE TITLE BASED IR
    #Get reciprocal rank
    f=open(".\IRtesting\IRCitationBased_RR.txt")
    lines=f.readlines()
    reciprocalRank_IRCitationBased=[float(e.strip()) for e in lines]
    reciprocalRank_IRCitationBased=np.asarray(reciprocalRank_IRCitationBased)
    #Get histogram
    title="Reciprocal Rank values distribution obtained with the CitationBased IR"
    xlabel="Reciprocal Rank"
    ylabel=""
    charts.histogram(reciprocalRank_IRCitationBased, title, xlabel, ylabel, n)
    mean=np.mean(reciprocalRank_IRCitationBased)
    MRR_IR2=np.mean(reciprocalRank_IRCitationBased)
    varRR_IR2=np.var(reciprocalRank_IRCitationBased, 1)
    #Get execution times
    f=open(".\IRtesting\IRCitationBased_MSEC.txt")
    lines=f.readlines()
    exeTimes_IRCitationBased=[float(e.strip()) for e in lines]
    exeTimes_IRCitationBased=np.asarray(exeTimes_IRCitationBased)
    #Get histogram
    title="Execution times distribution obtained with the CitationBased IR"
    xlabel="Execution time (msec)"
    ylabel=""
    charts.histogram(exeTimes_IRCitationBased, title, xlabel, ylabel, n)
    meanTime_IR2=np.mean(exeTimes_IRCitationBased)
    varTime_IR2=np.var(exeTimes_IRCitationBased, 1)
    
    #STUDENT-T TEST 
    #compare rr
    sp.ttest_rel(reciprocalRank_IRCitationBased, reciprocalRank_IRCitationBased, t_RR, p_RR)
    print "-----------Reciprocal ranks analysis-------------"
    print "MRR (IR Title Based): %.2f" % meanRR_IR1
    print "Variance of RR (IR Title Based): %.2f" % varRR_IR1
    print "MRR (IR Citation Based): %.2f" % meanRR_IR2
    print "Variance of RR (IR Citation Based): %.2f" % varRR_IR2
    print "t: %.2f" % t_RR
    print "p-value: %.2f" % p_RR
    print ""

    #compare execution times
    sp.ttest_rel(exeTimes_IRCitationBased, exeTimes_IRCitationBased, t_times, p_times)
    print "-----------Execution time analysis-------------"
    print "Mean execution time (IR Title Based): %.2f" % meanTime_IR1
    print "Variance of execution time (IR Title Based): %.2f" % varTime_IR1
    print "Mean execution time (IR Citation Based): %.2f" % meanTime_IR2
    print "Variance of execution time (IR Citation Based): %.2f" % varTime_IR2
    print "t: %.2f" % t_times
    print "p-value: %.2f" % p_times
    print ""


def analyze_dataset_generator():
    """
    This function analyses the size of the XML files generated and the time needed to generate them.
    The results obtained are plot. In addition, the two-way anova is applied to compare the results of
    three types of xml files.
    """
    #ANALYZE SIZE OF THE FILES
    sizeArticlesXML=[2.05, 4.26, 6.67, 8.65, 11.33, 12.14, 12.59, 15.08, 17.75, 20.66, 23.82, 27.10, 29.84, 33.08, 36.04, 37.75, 38.04, 38.60, 39.19, 39.93, 40.62, 40.83, 40.98, 41.13, 41.26, 41.41, 41.55, 41.69, 41.83, 41.97, 42.10, 42.62, 42.75, 42.87, 42.96, 43.06, 43.16, 43.28, 43.37, 43.47, 43.56, 43.66, 43.76, 43.87, 43.96, 44.07, 44.18, 44.28, 44.38 , 44.47, 44.57, 44.66, 44.75, 44.83, 44.93, 45.02, 45.10, 45.20, 45.29, 45.38, 45.47, 45.56, 45.64, 45.73, 45.82, 45.90, 45.99, 46.08, 46.17, 46.25, 46.34, 46.42, 46.51, 46.60, 46.69, 46.78, 46.88, 46.97, 47.07, 47.16, 47.25, 47.34, 47.42, 47.51, 47.59, 47.68, 47.76, 47.85, 47.93, 48.01, 48.09, 48.16, 48.25, 48.33, 48.41, 48.49, 48.58, 48.67, 48.75, 48.85, 48.92, 49.00, 49.08, 49.17, 49.26, 49.34, 49.43, 49.51, 49.60, 49.69, 49.76, 49.84, 49.93, 50.02, 50.11, 50.21, 50.30, 50.37, 50.46, 50.56, 50.64, 50.72, 50.82, 50.91, 51.01, 51.10, 51.19, 51.27, 51.36, 51.45, 51.53, 51.61, 51.69, 51.77, 51.86, 51.95, 52.03, 52.10, 52.19, 52.27, 52.34, 52.43, 52.51, 52.59, 52.66, 52.75, 52.84, 52.91, 53.00, 53.08, 53.17, 53.23, 53.32, 53.42, 53.50, 53.59, 53.68, 53.77, 53.85, 53.93, 54.03, 54.12, 54.20, 54.29, 54.38, 54.46, 54.55, 54.63, 54.70, 54.79, 54.87, 54.95, 55.03, 55.11, 55.19, 55.28, 55.35, 55.42, 55.50, 55.59, 55.66, 55.76, 55.84, 55.92, 56.00, 56.08, 56.16, 56.24, 56.32, 56.39, 56.47, 56.55, 56.62, 56.70, 56.78, 56.86, 56.95, 57.03, 57.12, 57.19, 57.27, 57.36, 57.45, 57.54, 57.63, 57.71, 57.79, 57.88, 57.96, 58.04, 58.12, 58.21, 58.29, 58.38, 58.45, 58.53, 58.61, 58.69, 58.77, 58.85, 58.93, 59.01, 59.09, 59.17, 59.24, 59.33, 59.41, 59.48, 59.57, 59.66,59.75, 59.84, 59.94, 60.03, 60.10, 60.18, 60.27, 60.36, 60.45, 60.55, 60.64, 60.72, 60.83, 60.92, 61.01, 61.10, 61.20, 61.27, 61.36, 61.44]
    sizeExtendedArticlesXML=[3.19, 5.85, 8.78, 12.23, 15.00, 15.86, 16.41, 19.05, 21.67, 23.99, 26.40, 28.91, 30.86, 34.00, 37.02, 40.31, 43.20, 46.01, 49.07, 52.47, 56.11, 56.79, 57.25, 57.67, 58.11, 58.60, 59.08, 59.50, 59.96, 60.47, 60.89, 62.65, 62.88, 63.11, 63.34, 63.57, 63.79, 64.01, 64.23, 64.46, 64.69, 64.92, 65.14, 65.38, 65.61, 65.84, 66.07, 66.30, 66.52, 66.75, 66.98, 67.21, 67.43, 67.66, 67.88, 68.11, 68.34, 68.56, 68.79, 69.01, 69.24, 69.46, 69.69, 69.92, 70.14, 70.37, 70.60, 70.83,71.06, 71.28, 71.52, 71.75, 71.99, 72.22, 72.46, 72.69, 72.93, 73.16, 73.39, 73.62, 73.86, 74.09, 74.33, 74.57, 74.80, 75.03, 75.27, 75.51, 75.74, 75.98, 76.21, 76.44, 76.68, 76.91, 77.15, 77.38, 77.62, 77.84, 78.09, 78.31, 78.55, 78.79, 79.02, 79.25, 79.48, 79.71, 79.94, 80.17, 80.41, 80.64, 80.87, 81.11, 81.34, 81.57, 81.82, 82.06, 82.30, 82.54, 82.77, 83.01, 83.25, 83.50, 83.75, 83.99, 84.22, 84.46, 84.70, 84.93, 85.17, 85.41, 85.65, 85.88, 86.12, 86.35, 86.59, 86.83, 87.07, 87.31, 87.54, 87.78, 88.02, 88.25, 88.49, 88.72, 88.96, 89.19, 89.43, 89.66, 89.89, 90.12, 90.37, 90.61, 90.85, 91.09, 91.33, 91.57, 91.80, 92.04, 92.27, 92.51, 92.75, 92.98, 93.22, 93.45, 93.69, 93.93, 94.17, 94.40, 94.63, 94.87, 95.10, 95.34, 95.57, 95.80, 96.04, 96.27, 96.51, 96.74, 96.98, 97.21, 97.45, 97.69, 97.92, 98.16, 98.39, 98.62, 98.86, 99.10, 99.34, 99.58, 99.82, 100.04, 100.29, 100.53, 100.76, 101.01, 101.24, 101.48, 101.72, 101.95, 102.79, 102.43, 102.67, 102.92, 103.17, 103.42, 103.66, 103.90, 104.15, 104.38, 104.62, 104.86, 105.11, 105.35, 105.59, 105.83, 106.07, 106.32, 106.56, 106.80, 107.04, 107.28, 107.52, 107.75, 107.99, 108.22, 108.47, 108.70, 108.95, 109.20, 109.44, 109.69, 109.94, 110.19, 110.44, 110.68, 110.93, 111.18, 111.44, 111.69, 111.94, 112.19, 112.44, 112.69, 112.94, 113.20, 113.454, 113.71, 113.97, 114.22]
    sizeCitationsXML=[11.65, 20.25, 31.02, 39.95, 41.06, 42.13, 43.10, 43.65, 45.11, 45.26, 45.33, 45.69, 46.57, 46.93, 47.64, 52.52, 57.49, 65.43, 73.85, 84.77, 96.45, 98.55, 99.93, 101.19, 102.48, 103.81, 105.17, 106.45, 107.81, 109.19, 110.37, 117.10, 117.66, 118.29, 118.82, 119.41, 119.99, 120.65, 121.23, 121.82, 122.47, 123.08, 123.69, 124.32, 124.96, 125.69, 126.34, 126.91, 127.49, 128.09, 128.67, 129.27, 129.84, 130.40, 131.06, 131.69, 132.29, 132.94, 133.54, 134.15, 134.75, 135.34, 135.92, 136.51, 137.11, 137.69, 138.29, 138.92, 139.54, 140.16, 140.80, 141.47, 142.25, 142.90, 143.58, 144.27, 144.94, 145.62, 146.27, 146.94, 147.60, 148.31, 148.97, 149.66, 150.25, 150.89, 151.58, 152.30, 152.90, 153.55, 154.28, 154.90, 155.66, 156.38, 157.02, 157.94, 158.30, 158.95, 159.58, 160.25, 160.95, 161.58, 162.24, 162.92, 163.60, 164.24, 164.90, 165.56, 166.26, 166.94, 167.54, 168.33, 168.96, 169.68, 170.43, 171.16, 171.90, 172.54, 173.19, 173.96, 174.67, 175.44, 176, 176.18, 176.91, 177.59, 178.31,  179.06, 179.71, 180.43, 181.13, 181.95, 182.62, 183.32, 183.99, 184.75, 185.52, 186.28, 186.91, 187.6, 188.30, 189.07, 189.70, 190.46, 191.07, 191.77, 192.43, 193.10, 193.75, 194.48, 195.19, 195.89, 196.54, 197.21, 197.96, 198.67, 199.35, 200.03, 200.70, 201.34, 202.07, 202.79, 203.40, 204.14, 204.83, 205.49, 206.17, 206.87, 207.48, 208.11, 208.78, 209.46, 210.14, 210.76, 211.41, 212.09, 212.74, 213.45, 214.1, 214.77, 215.45, 216.19, 216.85, 217.48, 218.18, 218.84, 219.46, 220.15, 220.81, 221.49, 222.16, 222.89, 223.48, 224.14, 224.85, 225.54, 226.28, 226.92, 227.61, 228.29, 228.90, 229.61, 230.3, 231.06, 231.81, 232.57, 233.30, 234.02, 234.70, 235.51, 236.19, 236.83, 237.54, 238.26, 238.92, 239.64, 240.39, 241.06, 241.85, 242.56, 243.25, 243.93, 244.56, 245.21, 245.86, 246.53, 247.18, 247.87, 248.50, 249.30, 250.04, 250.83, 251.62, 252.40, 253.21, 253.99, 254.70, 255.42,256.021, 257.01, 257.82, 258.59, 259.31, 260.07, 260.76, 261.56, 262.434, 263.24, 263.97, 264.84]
    #Plot results
    n=len(sizeArticlesXML)
    y_list=[[], [], []]
    y_list[0]=sizeArticlesXML
    y_list[1]=sizeExtendedArticlesXML
    y_list[2]=sizeCitationsXML
    x_list=[[], [],[]]
    x_list[0]=range(100, (n+1)*100, 100)
    x_list[1]=range(100, (n+1)*100, 100)
    x_list[2]=range(100, (n+1)*100, 100)
    title="Analysis of the size of XML files"
    xlabel="Number of articles in the XML files"
    ylabel="Size of the XML file (kb)"
    labels=[[], [], []]
    labels[0]="Articles.xml file"
    labels[1]="ExtendedArticles.xml file"
    labels[2]="Citations.xml file"
    plt.figure()
    charts.multiple_line_chart(x_list, y_list, title, xlabel, ylabel, labels, lineTypes=['o-', 'o-', 'o-'])
    plt.show()
    #Two-way ANOVA analysis
    nGroups=5
    sizeG=int(n/nGroups)
    r=range(n)
    Y=[]
    A=[]
    B=[]
    for i in r:
        Y.append(sizeArticlesXML)
        Y.append(sizeExtendedArticlesXML)
        Y.append(sizeCitationsXML)
        A.append(1)
        A.append(2)
        A.append(3)
        for g in range(1, nGroups+1):
            if i < (g*sizeG):
                B.append(g)
                B.append(g)
                B.append(g)
                break
            else:
			    pass
       
    FF  = spm1d.stats.anova2(Y, A, B, equal_var=True)
    FFi = [F.inference(alpha=0.05)   for F in FF]
    plt.figure()
    FFi[0].plot()   #Factor A main effect
    FFi[1].plot()   #Factor B main effect
    FFi[2].plot()   #Interaction effect
    plt.show()

    #ANALYZE EXECUTION TIME
    #Get execution times
    f=open(".\XMLGenerator\ArticlesTimes.txt")
    lines=f.readlines()
    timesArticlesXML=[float(e.strip()) for e in lines]
    timesArticlesXML=np.asarray(timesArticlesXML)
    f=open(".\XMLGenerator\CitationsTimes.txt")
    lines=f.readlines()
    timesCitationsXML=[float(e.strip()) for e in lines]
    timesCitationsXML=np.asarray(timesCitationsXML)
    f=open(".\XMLGenerator\ExtendedArticlesTimes.txt")
    lines=f.readlines()
    timesExtendedArticlesXML=[float(e.strip()) for e in lines]
    timesExtendedArticlesXML=np.asarray(timesExtendedArticlesXML)
    #Plot results
    n=len(timesArticlesXML)
    y_list=[[], [], []]
    y_list[0]=timesArticlesXML
    y_list[1]=timesExtendedArticlesXML
    y_list[2]=timesCitationsXML
    x_list=[[], [],[]]
    x_list[0]=range(100, (n+1)*100, 100)
    x_list[1]=range(100, (n+1)*100, 100)
    x_list[2]=range(100, (n+1)*100, 100)
    title="Analysis of the time needed to generate of XML files"
    xlabel="Number of articles in the XML files"
    ylabel="Execution time (ms)"
    labels=[[], [], []]
    labels[0]="Articles.xml file"
    labels[1]="ExtendedArticles.xml file"
    labels[2]="Citations.xml file"
    plt.figure()
    charts.multiple_line_chart(x_list, y_list, title, xlabel, ylabel, labels, lineTypes=['o-', 'o-', 'o-'])
    plt.show()
    #Two-way ANOVA analysis
    nGroups=5
    sizeG=int(n/nGroups)
    r=range(n)
    Y=[]
    A=[]
    B=[]
    for i in r:
        Y.append(timesArticlesXML)
        Y.append(timesExtendedArticlesXML)
        Y.append(timesCitationsXML)
        A.append(1)
        A.append(2)
        A.append(3)
        for g in range(1, nGroups+1):
            if i < (g*sizeG):
                B.append(g)
                B.append(g)
                B.append(g)
                break
            else:
			    pass
       
    FF  = spm1d.stats.anova2(Y, A, B, equal_var=False)
    FFi = [F.inference(alpha=0.05)   for F in FF]
    FFi[0].plot()   #Factor A main effect
    FFi[1].plot()   #Factor B main effect
    FFi[2].plot()   #Interaction effect
        		
def analyze_parser():
    """
    This function analyses and compares the times needed to parse the articles and the
    times needed to parse the files and add them to the database.
    The results are plotted and the Student's t-test is applied.
    """
    #GET DATA FROM FILES
    f=open("./Parsing/parsingTimes_WB.txt")
    lines=f.readlines()
    parsingTimes=[int(e.strip()) for e in lines]
    parsingTimes=np.asarray(parsingTimes)

    f=open("./Parsing/processingTimes_WB.txt")
    lines=f.readlines()
    processingTimes=[int(e.strip()) for e in lines]
    processingTimes=np.asarray(processingTimes)

    #GET DISTRIBUTIONS
    n=500
    title="Distribution of parsing times"
    xlabel="Parsing time (ms)"
    ylabel=""
    plt.figure()
    charts.histogram(parsingTimes, title, xlabel, ylabel, n)
    mean=np.mean(parsingTimes)
    var=np.var(parsingTimes, ddof=1)
    print "Mean parsing time: %.5f" % mean
    print "min parsing time: %.5f" % min(parsingTimes)
    print "max parsin time: %.5f" % max(parsingTimes)
    print "Vanriance of parsing time: %.3f" % var

    title="Distribution of processing times times"
    xlabel="Processing time (ms)"
    ylabel=""
    plt.figure()
    charts.histogram(processingTimes, title, xlabel, ylabel, n)
    mean=np.mean(processingTimes)
    var=np.var(processingTimes, ddof=1)
    print "Mean processing time: %.9f" % mean
    print "min processsinf time: %.5f" % min(processingTimes)
    print "max processsinf time: %.5f" % max(processingTimes)
    print "Vanriance of processing time: %.3f" % var

    #T-TEST
    t_times, p_times=sp.ttest_rel(parsingTimes, processingTimes)
    print "t: %.2f" % t_times
    print "p-value: %.20f" % p_times
    print "%d" % len(parsingTimes)
 	
   


def analyze_network():
    """
    This function creates a digraph with the articles of the dataset and their refereces. The order, 
    size, cluster coefficient and in-degree distribution is obtained.
    """
    #get references from db
    references=connect_db()
    #create graph
    G=nx.DiGraph()
    G.add_edges_from(references)
    print "Number of nodes: %d" % G.number_of_nodes()
    print "Number of edges: %d" % G.number_of_edges()
    print "Density: %.10f" % nx.density(G)

    analyze_degree_distribution(G)
    #analyze_assortativity_distribution(G)
    analyze_clustering_coefficient_distribution(G)
   
def analyze_degree_distribution(G):
    """
    This function gets the in-degree distribution of the given digraph and computes the mean and variance.
    """
    n=500
    degrees=G.in_degree().values()
    #Get histogram
    #degrees=[x for x in degrees if]
    title="Distribution of nodes according to their degree"
    xlabel="Degree of the node"
    ylabel=""
    #print degrees
    charts.histogram(degrees, title, xlabel, ylabel, n, yLog=True)
    mean=np.mean(degrees)
    var=np.var(degrees, ddof=1)
    print "Mean degree: %.3f" % mean
    print "Vanriance of degree: %.3f" % var


def analyze_clustering_coefficient_distribution(G):
    """
    Gets the distribution of nodes according to their clustter coefficient and computes the mean and average
    """
    n=50
    cluster_coef=get_cluster_coefficients(G)
    #Get histogram
    title="Distribution of nodes according to their clustering coefficient"
    xlabel="Clustering coefficient"
    ylabel=""
    #print degrees
    charts.histogram(cluster_coef, title, xlabel, ylabel, n)
    mean=np.mean(cluster_coef)
    print "Mean cluster coefficient: %.3f" % mean

def get_cluster_coefficients(G):
    """
    Computes the cluster coefficient of the nodes of the given digraph
    """
    nodes=G.nodes()
    coefs=[]
    for node in nodes:
        #COMPUTE CLUSTER COEFFICIENT
        #Get number of connections between neighbours
        neighbors=[neig for neig in nx.all_neighbors(G, node)]
        
        edges=G.out_edges(neighbors)
        nConn=0
        for e in edges:
            if (e[1] in neighbors):
                nConn+=1
        #Get max possible numeber of connections
        maxConn=(len(neighbors))*(len(neighbors)-1)
        #Get coef
        if(maxConn!=0):
            coef=float(nConn)/float(maxConn)

        #ADD TO THE LIST

        coefs.append(coef)
    return coefs

def connect_db():
    """
    Creates a connection with the database and gets the references
    """
    #CREAR LA CONEXION CON LA DB
    datos_conexion=[DB_HOST, DB_USER, DB_PASS, DB_NAME]
    conn = MySQLdb.connect(*datos_conexion)
    cur = conn.cursor()
    #BUSCAR EN LA DB
    query='SELECT source_article_id, referenced_article_id FROM reference;'
    if cur.execute(query)==0:
            return False
    references = cur.fetchall()
    return references






if __name__=='__main__':
    main()



