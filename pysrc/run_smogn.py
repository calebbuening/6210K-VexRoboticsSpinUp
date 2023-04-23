import pandas
import smogn
import seaborn

df = pandas.read_csv("data.csv")

df_smogn = smogn.smoter(data=df, y='Speed')

seaborn.kdeplot(df['SalePrice'], label = "Original")
seaborn.kdeplot(df_smogn['SalePrice'], label = "Modified")
