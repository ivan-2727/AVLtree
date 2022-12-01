package main
import (
	"fmt"
 	"math/rand"
	"time"
	"os"
)

type Vertex struct {
	left *Vertex
    right *Vertex
    par *Vertex
	isLeft bool
	left_size int
	right_size int

    key int
	val int
	cnt int 
}

func makeVertex(key int, isLeft bool) *Vertex {
	var v Vertex
	v.left = nil
	v.right = nil
	v.par = nil
	v.isLeft = isLeft
	v.left_size = 0
	v.right_size = 0

	v.key = key
	v.cnt = 1

	return &v
} 

func leftmost(v *Vertex) *Vertex {
	if v==nil {return nil}
	for true {
		if v.left != nil {
			v = v.left
		} else {
			return v
		}
	}
	return nil 
}

func rightmost(v *Vertex) *Vertex {
	if v==nil {return nil}
	for true {
		if v.right != nil {
			v = v.right
		} else {
			return v
		}
	}
	return nil 
}

func next(v *Vertex) *Vertex {
	if v==nil {return nil}
	if v.right != nil {return leftmost(v.right)}
	for true {
		ok := false
		if v==nil {return nil}
		if v.isLeft {ok = true}
		v = v.par 
		if ok {return v}
	}
	return nil 
}

type TreeMap struct {
	head *Vertex 
}

func makeTreeMap() TreeMap {
	var data TreeMap
	data.head = nil
	return data
}

func basic_insert(key int, v *Vertex) *Vertex {
	for true {
		if key == v.key {
			v.cnt++
			return v
		} else if key < v.key {
			v.left_size++
			if v.left != nil {
				v = v.left
			} else {
				v.left = makeVertex(key, true)
				v.left.par = v
				return v.left
			}
		} else {
			v.right_size++
			if v.right != nil {
				v = v.right
			} else {
				v.right = makeVertex(key, false)
				v.right.par = v
				return v.right
			}
		}
	}
	return nil
}

func basic_find(key int, v *Vertex) *Vertex {
	for true {
		if v==nil {return nil}
		if key == v.key {
			return v
		} else if key < v.key {
			v = v.left
		} else {
			v = v.right 
		}
	}
	return nil 
}

func dfs(v *Vertex) {
	if v==nil {return}
	fmt.Printf("Key: %d\n", v.key)
	if v.par != nil {
		fmt.Printf("Parent: %d\n", v.par.key)
	} else {
		fmt.Printf("Parent: -1\n")
	}
	if v.left != nil {
		fmt.Printf("Left: %d\n", v.left.key)
	} else {
		fmt.Printf("Left: -1\n")
	}
	if v.right != nil {
		fmt.Printf("Right: %d\n", v.right.key)
	} else {
		fmt.Printf("Right: -1\n")
	}
	fmt.Printf("left_size: %d\n", v.left_size)
	fmt.Printf("right_size: %d\n", v.right_size)
	fmt.Printf("----------------------\n")
    dfs(v.left)
    dfs(v.right)
}

func (this *TreeMap) rotate_left(z *Vertex) {
	y := z.right
	z.right = y.left
	z.right_size = y.left_size 
	if y.left != nil {
		y.left.par = z
		y.left.isLeft = false
	}
	y.left = z
	y.left_size = z.right_size + 1 + z.left_size
	y.par = z.par 
	z.par = y
	y.isLeft = z.isLeft
	z.isLeft = true 
	if y.par == nil {
		this.head = y
	} else {
		if y.isLeft {
			y.par.left = y
		} else {
			y.par.right = y
		} 
	}
}

func (this *TreeMap) rotate_right(z *Vertex) {
	y := z.left
	z.left = y.right
	z.left_size = y.right_size 
	if y.right != nil {
		y.right.par = z
		y.right.isLeft = true
	}
	y.right = z
	y.right_size = z.left_size + 1 + z.right_size
	y.par = z.par 
	z.par = y
	y.isLeft = z.isLeft
	z.isLeft = false 
	if y.par == nil {
		this.head = y
	} else {
		if y.isLeft {
			y.par.left = y
		} else {
			y.par.right = y 
		}
	}
}

func abs(x int) int {
	if x < 0 {return -x}
	return x
}

func (this *TreeMap) rebalance(v *Vertex) {
	if v==nil {return} 
	var x, y, z *Vertex
	x = v
	if x.par != nil {
		y = x.par
	} else {return}
	if y.par != nil {
		z = y.par
	} else {return}

	for true {
		if z==nil {return}
		if abs(z.left_size - z.right_size) > 1 {break}
		tmp := y
		y = z
		x = tmp
		z = z.par 
	}
		
	if y.isLeft && x.isLeft {
		this.rotate_right(z)
	} else if y.isLeft && !x.isLeft {
		this.rotate_left(y)
		this.rotate_right(z)
	} else if !y.isLeft && x.isLeft {
		this.rotate_right(y)
		this.rotate_left(z)
	} else {
		this.rotate_left(z) 
	}
}

func (this *TreeMap) insert(key int) {
	if this.head == nil {
		this.head = makeVertex(key, true) 
		return 
	}
	this.rebalance(basic_insert(key, this.head))
}

func (this *TreeMap) find(key int) *Vertex {
	if this.head == nil {return nil}
	return basic_find(key, this.head)
}

func (this *TreeMap) min() *Vertex {
	return leftmost(this.head) 
}

func (this *TreeMap) max() *Vertex {
	return rightmost(this.head) 
}
 
func main() {
	f, _ := os.Create("result.txt")
	for tot := 100; tot <= int(1e8); tot *= 10 {
		data := makeTreeMap()
		x := []int{}
		for i := 0; i < tot; i++ {x = append(x, rand.Intn(1000))}
		t0 := time.Now()
		for i := 0; i < tot; i++ {data.insert(x[i])}
		a := time.Since(t0).Milliseconds()
		t1 := time.Now()
		for i := 0; i < tot; i++ {
			v := data.find(x[i])
			if v == nil {
				fmt.Println("Something is wrong")
			}
		}
		b := time.Since(t1).Milliseconds()
		_, _ = f.WriteString(fmt.Sprintf("%d %d\n", a, b))
	}
}