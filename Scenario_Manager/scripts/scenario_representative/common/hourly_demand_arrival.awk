BEGIN{
    sum_departed = 0;
    sum_arrived = 0;
    line = 0;
    FS = ",";
}
{
    sum_departed += $4;
    sum_arrived += $5;
    line++;
    if (line == 12){
        printf("%s,%lf,%lf\n", $1, sum_departed, sum_arrived);
        line = 0;
        sum_departed = 0;
        sum_arrived = 0;
    }
}
