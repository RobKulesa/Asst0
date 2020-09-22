import subprocess 

print ("Hey this is Python Autograder Script Running\n") 

subprocess.call(["make"])

myfile = open("testcases.txt", "rt")
contents = myfile.read()         
myfile.close()

args = contents.split('\n')
count = 1
for x in args:
    print("---------TestCase %d------------" %(count))
    print("STRING: " + x + "\n")
    count+=1
    subprocess.call(["./Asst0", x])

subprocess.call(["make","clean"])