FROM scratch
ADD ads:v1 /
ADD ads.txt /
ADD slots.txt / 
CMD ["/ads:v1"]
