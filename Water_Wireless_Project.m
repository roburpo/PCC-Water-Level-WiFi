%% Group Project
% * Authors: ENGR114 Winter 2018 Student Group
% * Group Members: Robert English, Anh Bui, Peter Polyakov, Kendra Young.
% * Date: Mar 20, 2018
% * Descriptive: This MATLAb code to pull down the water level data from
%                ThinkSpeak.com, clean up and analyze the data.
 

%% Clear all the command window, variable workspace, and close all the window
clc; clear; close all;

%% Get the water level data from ThinkSpeak.com

data_num = input('\n Enter how many data you want to measure: ')

raw_data = webread(sprintf('https://api.thingspeak.com/channels/448325/fields/1.csv?results=%d', data_num));


%% Clean up the data to get the water level

data = table2array(raw_data(:,3))             % pull out the third column from the data 

x_values = [1:numel(data)]';                  % determine the values for x-axis

y_str = '                ';                   % create an empty string with 1 row x 16 cols

for i=1 : numel(data)                         % use the for loop to run the data order
    
    if numel(data{i}) == 17                   % use the if function to check if the y values is over 10000
        
        y_add = data{i}(1:16);                % truncate the last digit data
         
        y_str = [y_str; y_add];               % show the table of the strings containing the y values
        
        y_values = str2num(y_str(:, 8:end));  % adding y_value at the index to the y_values matrix
        
    else                                      % if else the y values is less than 10000k
        y_add = data{i}(1:16);                % adding one row for the string after the index ran
        
        y_str = [y_str; y_add];               % show the table of the strings containing the y values
        
        y_values = str2num(y_str(:, 8:end));  % adding y_value at the index to the y_values matrix
    end
    
end 
 y_values = y_values;                         % combine all the y values
 
 y_values = (y_values - 4000)/7742;           % convert the resistance values to the water height values
 
 table = [x_values y_values]                  % create a table of x and y values
        

%% Plot the water level vs. time recorded

figure(1)                                     % create a figure 1

plot(x_values, y_values)                      % plot the x and y values 

ylabel(' Water height (in)')                  % add name to the y-axis

xlabel(' Data points')                        % add name to the x-axis

title(' Water Sensor Level')                  % add title to the plot
  


