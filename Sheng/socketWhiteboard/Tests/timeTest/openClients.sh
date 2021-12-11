lineTp=$(find ./lineTp -name 'Whiteboard')
dotTp=$(find ./dotTp -name 'Whiteboard')

for i in $(seq 1 5)
do
    $lineTp &
    sleep 0.1
    $dotTp &
    sleep 0.1
done
