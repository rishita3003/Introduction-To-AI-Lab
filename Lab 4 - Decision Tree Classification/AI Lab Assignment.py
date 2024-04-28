#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
from math import log2
import pandas as pd


# In[18]:


data={'Alt':['T','T','F','T','T','F','F','F','F','T','F','T'],'Bar':['F','F','T','F','F','T','T','F','T','T','F','T'],'Fir': ['F','F','F','T','T','F','F','F','T','T','F','T'],'Hun':['T','T','F','T','F','T','F','T','F','T','F','T'],'Pat':['Some','Full','Some','Full','Full','Some','None','Some','Full','Full','None','Full'],'Price':['$$$','$','$','$','$$$','$$','$','$$','$','$$$','$','$'],'Rain':['F','F','F','F','F','T','T','T','T','F','F','F'],'Res':['T','F','F','F','T','T','F','T','F','T','F','F'],'Type':['French','Thai','Burger','Thai','French','Italian','Burger','Thai','Burger','Italian','Thai','Burger'],'Est':['0-10','30-60','0-10','10-30','>60','0-10','0-10','0-10','>60','10-30','0-10','30-60'],'Wait':['T','F','T','T','F','T','F','T','F','F','F','T']}


# In[19]:


df=pd.DataFrame(data)
df


# In[20]:


#entropy calculation of an attribute
#entropy corresponds to the randomness
def entropy(attribute):
    #print(attribute)
    elements,count=np.unique(attribute,return_counts=True)
    length=len(elements)
    #print(length)
    total_count=0
    for i in range(length):
        total_count+=count[i]
    
    entropyy=0
    for i in range(length):
        p=count[i]/total_count
        entropyy+=(-1*p)*log2(p)
        
    return entropyy


# In[21]:


print(entropy(df['Wait'])) #log base 2 remember


# In[24]:


def InfoGain(data,split_attribute,target_name='Wait'):
    total_entropy=entropy(data[target_name])
    vals,counts=np.unique(data[split_attribute],return_counts=True)
    target_vals,target_counts=np.unique(data[target_name],return_counts=True)
    num_attr=len(vals)
    target_num_attr=len(target_vals)
    weighted_entropy=0
    for i in range(num_attr):
        data_subset=data[data[split_attribute]==vals[i]] #as we right now want to deal with only one type of the particular attribute
        subset_entropy=0
        for j in range(target_num_attr):
            count_target_value=len(data_subset[data_subset[target_name]==target_vals[j]])
            p_target_value=count_target_value/counts[i] 
            if p_target_value>0:
                subset_entropy-=p_target_value*log2(p_target_value)
                
        weighted_entropy+=(counts[i]/np.sum(counts))*subset_entropy
        
    information_gain=total_entropy-weighted_entropy
    return information_gain


# In[25]:


#choose the best attribute as a root for splitting further
def choose_best_attribute(data,attributes,target_attribute):
    information_gains=[]
    for attr in attributes:
        information_gains.append(InfoGain(data,attr,target_attribute))
        
    best_attr_index=np.argmax(information_gains)
    return attributes[best_attr_index]


# In[26]:


best_attribute=choose_best_attribute(df,df.columns[:-1],'Wait')
print(f"The best attribute to split on is: {best_attribute}")


# In[58]:


class DecisionTree:
    def __init__(self,attribute):
        self.attribute=attribute
        self.children={}
        self.is_leaf=False
        self.classification=None
        
    def add_child(self,val,node):
        self.children[val]=node
        


# In[63]:


def build_tree(data,attributes,target_attr):
    #base case, if leaf node is reached -- all target values now have same values
    #print(type(len(np.unique(data[target_attr]))))
    #print(type(1))
    if len(np.unique(data[target_attr])) <= 1: 
        #when in the data, all the target values have the same values, that means we have classified so stop
        leaf_node=DecisionTree(None)
        leaf_node.is_leaf=True
        leaf_node.classification=data[target_attr].iloc[0]
        return leaf_node
    
    elif len(data)==0 or len(attributes)==0:
        leaf_node=DecisionTree(None)
        leaf_node.is_leaf=True
        leaf_node.classification = np.unique(df[target_attr])[np.argmax(np.unique(df[target_attr], return_counts=True)[1])]
        return leaf_node
    
    else:
        best_attr=choose_best_attribute(data,attributes,target_attr)
        tree=DecisionTree(best_attr) #the best attribute chosen is now the root of the tree
        attributes=[attr for attr in attributes if attr!=best_attr] #all the remaining attributes
        
        for val in np.unique(data[best_attr]):
            sub_data = data[data[best_attr] == val].reset_index(drop=True) # Correctly create a sub-dataset for seperate types in the chosen best attribute
            subtree=build_tree(sub_data,attributes,target_attr) #build tree further for this data set created with the attributes apart from the best attr chosen
            tree.add_child(val,subtree)
            
        return tree
        


# In[66]:


def print_tree(node, level=0, parent_attr=None, parent_value=None):
    indent = " " * (level * 4)  # Using 4 spaces for each level of indentation
    if parent_attr is not None and parent_value is not None:
        print(f"{indent}[{parent_attr} = {parent_value}]")
    if node.is_leaf:
        print(f"{indent}-> Classification: {node.classification}")
    else:
        print(f"{indent}Node: {node.attribute}")
        for value, child in node.children.items():
            print_tree(child, level+1, parent_attr=node.attribute, parent_value=value)
            
attributes=list(df.columns[:-1])
decision_tree=build_tree(df,attributes,"Wait")
print_tree(decision_tree)


# In[ ]:




