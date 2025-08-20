Data structure evaluation 

File read (without creating course Objects): 

Big O of the read algorithm = O(N).  

The reading logic loops over the input twice. Once to get prerequisites and another time to load the data structure. This leaves us with N + N  as the run time. But since only the highest order term is kept when terms are summed, we are left with just N. The file read is method is the same for all data structures. 

Binary Search Tree  - Create and add course object 

Big O (per single course) = O(N) 

The Binary Search Tree algorithm first checks the prerequisites. At worse case this can take N operations. Then it finds the where to insert the node in the tree, which can take Log N operations. This leaves us with Log N + N which simplifies to just N. 

Hash Table - Create and add course Object 

Big O (per single course) = O(N) 

The Hash Table algorithm also first checks the prerequisites N. Then it finds where to insert the course within the Hash Table, N. This leaves us with N + N, which also simplifies to just N. 

Vector - Create and add course Objects 

Big O (per single course) = O(N). 

The vector algorithm still has to check the prerequisities. This can take a many as N iterations. But after the check, adding the course should only take 1 iteration. Leaving us with N + 1, which also simplifies to N. 

Recommendation Considerations 

Please note that despite the fact the big O notation of all three algorithms boils down to N, the vector addition is much faster than the other two algorithms, and the Binary Search Tree algorithm is much faster the Hash Table algorithm. But there is more than addition speed to consider when choosing a data structure. Search speed and sort speed is also important. In these areas the Hash Table and Binary Search Tree have a huge advantage over the Vector. So a proper recommendation comes down to what the expected use of the data structure is.  

Recommendation 

Best Suited Algorithm = Binary Search Tree 

The situation described by the prompt calls for more search, sort, and print operations than course creation/addition operations. So the Vector is out. And the Binary Search Tree can find courses much faster than a Hash Table, giving it the edge. 
