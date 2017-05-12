d <- read.csv("results_plot.csv", sep=";", header=T)
d <- d[d$TIME < 800,]
d$TYPE[d$TYPE==0] <- "new"
d$TYPE[d$TYPE==1] <- "random"
d$TYPE[d$TYPE==2] <- "min_work"
d2 <- d
d <- d[d$GROUP==16,]

library("ggplot2")

p_res <- ggplot( d[d$TASK_COUNT==100 & d$CPU_A==10 & d$CPU_B==10 | d$CPU_A==100 & d$CPU_B==100,], aes(x=SUM_L, y=LAMBDA) )
p_res + geom_point(aes(colour=TYPE, shape=TYPE)) + facet_wrap(~ CPU_A + CPU_B)

ggsave("plot_lambda1.png", width=2*par("din")[1])

p_res2 <- ggplot( d[d$TASK_COUNT==100 & d$CPU_A==100 & d$CPU_B==10,], aes(x=SUM_L, y=LAMBDA) )
p_res2 + geom_point(aes(colour=TYPE, shape=TYPE)) + facet_wrap(~ CPU_A + CPU_B)
ggsave("plot_lambda2.png")

p_res3 <- ggplot( d[d$TASK_COUNT==1000 & d$CPU_A==100 & d$CPU_B==10,], aes(x=SUM_L, y=LAMBDA) )
p_res3 + geom_point(aes(colour=TYPE, shape=TYPE)) + facet_wrap(~ CPU_A + CPU_B)
ggsave("plot_lambda3.png")

p_res4 <- ggplot( d[d$TASK_COUNT==10 & d$CPU_A==100 & d$CPU_B==10,], aes(x=SUM_L, y=LAMBDA) )
p_res4 + geom_point(aes(colour=TYPE, shape=TYPE)) + facet_wrap(~ CPU_A + CPU_B)
ggsave("plot_lambda4.png")

p_time <- ggplot( d2[d2$TASK_COUNT==100,], aes(x=TYPE, y=TIME) )
p_time + geom_boxplot()

ggsave("plot2.png")

p_time <- ggplot( d2[d2$TASK_COUNT==1000,], aes(x=TYPE, y=TIME) )
p_time + geom_boxplot()

ggsave("plot3.png")
