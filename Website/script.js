function runCode() {
    const code = document.getElementById("code").value;

    const registerMapping = new Map([
        ["R0", "0000000000000000"],
        ["R1", "0000000000000000"],
        ["R2", "0000000000000000"],
        ["R3", "0000000000000000"],
        ["R4", "0000000000000000"],
        ["R5", "0000000000000000"],
        ["R6", "0000000000000000"],
        ["R7", "0000000000000000"]
    ]);

    const dataMemory = new Map();
    const compilerVariables = new Map();

    let lineAddress = 0;
    let jumptoAddress = false;
    let addressToJump = 0;
    let previousOperationOutput = "";

    const rawLines = code.split(/\r?\n/);
    const lines = [];

    for (let line of rawLines) {
        if (line.trim() === "") {
            continue;
        }

        const trimmed = line.trim();

        if (trimmed.startsWith("label ")) {
            const labelName = trimmed.substring(6).trim();
            compilerVariables.set(labelName, lineAddress);
            continue;
        }

        lines.push(line);
        lineAddress++;
    }

    const toUint16 = (val) => val & 0xFFFF;
    const toBinaryString = (val) => (val & 0xFFFF).toString(2).padStart(16, '0');

    for (let i = 0; i < lines.length; i++) {
        let line = lines[i];

        const args = line.trim().split(/\s+/);

        if (jumptoAddress) {
            i = addressToJump - 1;
            jumptoAddress = false;
            continue;
        }

        if (args[0] === "ldr") {
            const address = parseInt(registerMapping.get(args[2]), 2);
            registerMapping.set(args[1], dataMemory.get(address) || "0000000000000000");
        }
        else if (args[0] === "str") {
            const address = parseInt(registerMapping.get(args[2]), 2);
            dataMemory.set(address, registerMapping.get(args[1]));
        }
        else if (args[0] === "db") {
            const val = parseInt(args[2].substring(1));
            registerMapping.set(args[1], toBinaryString(val));
        }
        else if (args[0] === "sb") {
            const address = parseInt(registerMapping.get(args[1]), 2);
            const val = BigInt(args[2].substring(1));
            dataMemory.set(address, toBinaryString(Number(val & 0xFFFFn)));
        }
        else if (args[0] === "arithmetic") {
            if (args[1] === "and") {
                const A = parseInt(registerMapping.get(args[2]), 2);
                const B = parseInt(registerMapping.get(args[3]), 2);

                previousOperationOutput = toBinaryString(A & B);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "or") {
                const A = parseInt(registerMapping.get(args[2]), 2);
                const B = parseInt(registerMapping.get(args[3]), 2);

                previousOperationOutput = toBinaryString(A | B);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "xor") {
                const A = parseInt(registerMapping.get(args[2]), 2);
                const B = parseInt(registerMapping.get(args[3]), 2);
                previousOperationOutput = toBinaryString(A ^ B);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "not") {
                const A = parseInt(registerMapping.get(args[2]), 2);

                previousOperationOutput = toBinaryString(~A);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "neg") {
                const A = toUint16(parseInt(registerMapping.get(args[2]), 2));
                
                previousOperationOutput = toBinaryString(-A - 1);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "add") {
                const A = toUint16(parseInt(registerMapping.get(args[2]), 2));
                const B = toUint16(parseInt(registerMapping.get(args[3]), 2));
                previousOperationOutput = toBinaryString(A + B);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "sub") {
                const A = toUint16(parseInt(registerMapping.get(args[2]), 2));
                const B = toUint16(parseInt(registerMapping.get(args[3]), 2));
                previousOperationOutput = toBinaryString(A - B);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "mul") {
                const A = toUint16(parseInt(registerMapping.get(args[2]), 2));
                const B = toUint16(parseInt(registerMapping.get(args[3]), 2));

                previousOperationOutput = toBinaryString(A * B);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "div") {
                const A = toUint16(parseInt(registerMapping.get(args[2]), 2));
                const B = toUint16(parseInt(registerMapping.get(args[3]), 2));

                previousOperationOutput = toBinaryString(Math.floor(A / B));
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "inc") {
                const A = toUint16(parseInt(registerMapping.get(args[2]), 2));

                previousOperationOutput = toBinaryString(A + 1);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "dec") {
                const A = toUint16(parseInt(registerMapping.get(args[2]), 2));

                previousOperationOutput = toBinaryString(A - 1);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "shl") {
                const A = parseInt(registerMapping.get(args[2]), 2);
                const B = parseInt(registerMapping.get(args[3]), 2);
                const lastFour = B & 0x0F;

                previousOperationOutput = toBinaryString(A << lastFour);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "shr") {
                const A = parseInt(registerMapping.get(args[2]), 2);
                const B = parseInt(registerMapping.get(args[3]), 2);
                const lastFour = B & 0x0F;

                previousOperationOutput = toBinaryString(A >>> lastFour);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "cmp") {
                const A = registerMapping.get(args[2]);
                const B = registerMapping.get(args[3]);
                previousOperationOutput = (A === B) ? "0000000000000000" : "0000000000000001";
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "sar") {
                const binaryA = registerMapping.get(args[2]);

                let A = parseInt(binaryA, 2);
                if (binaryA[0] === '1') A -= 0x10000; 

                const B = parseInt(registerMapping.get(args[3]), 2);
                const lastFour = B & 0x0F;

                previousOperationOutput = toBinaryString(A >> lastFour);
                registerMapping.set(args[2], previousOperationOutput);
            }
            else if (args[1] === "ror") {
                const A = parseInt(registerMapping.get(args[2]), 2);
                const B = parseInt(registerMapping.get(args[3]), 2);
                const shift = B & 0x0F;

                if (shift === 0) {
                    previousOperationOutput = toBinaryString(A);
                }
                else {
                    const rotated = ((A >>> shift) | (A << (16 - shift)));
                    previousOperationOutput = toBinaryString(rotated);
                }
                registerMapping.set(args[2], previousOperationOutput);
            }
        }
        else if (args[0] === "jmp") {
            const label = args[1].substring(1);
            addressToJump = compilerVariables.get(label);
            jumptoAddress = true;
        } 
        else if (args[0] === "jz") {
            if (previousOperationOutput === "0000000000000000") {
                const label = args[1].substring(1);
                addressToJump = compilerVariables.get(label);
                jumptoAddress = true;
            }
        }
        else if (args[0] === "label") {
            continue;
        }
        else if (args[0] === "clear") {
            registerMapping.set(args[1], "0000000000000000");
        }
        else {
            if (args[0] === "end") {
                break;
            }
            else {
                console.log("Encountered an error");
                break;
            }
        }
    }

    const outputDiv = document.getElementById("codeoutput");
    outputDiv.replaceChildren();
    
    const sortedDataMemory = new Map([...dataMemory.entries()].sort((a, b) => a[0] - b[0]));

    sortedDataMemory.forEach((value, address) => {
        const newP = document.createElement("p");
        newP.textContent = `${address.toString().padStart(4, '0')}: ${value}`;
        outputDiv.appendChild(newP);

        const newP2 = document.createElement("p");
        newP2.textContent = `${address.toString().padStart(4, '0')}: ${parseInt(value, 2)}`;
        outputDiv.appendChild(newP2);
    });
}