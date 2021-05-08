width=property.getNumber("width")
height=property.getNumber("height")
upscale=property.getNumber("upscale")
portD=property.getNumber("port")

total = width * height * 6

response = ""
tick=1
loaded=false

async.httpGet(portD,"/") 

function onDraw()
	if(loaded) then
		async.httpGet(portD,"/")

        local xPP=1
        local yPP=1
        
        for pB,pG,pR in response:gmatch("(%w%w)(%w%w)(%w%w)") do
            screen.setColor(tonumber(pR,16),tonumber(pG,16),tonumber(pB,16))
            screen.drawRectF((xPP-1)*upscale,(yPP-1)*upscale,upscale,upscale)
            if(xPP==width) then
                xPP=0
                yPP=yPP+1
                if(yPP==height+1) then break end
            end
            xPP=xPP+1
        end

	end
end

function httpReply(port, request_body, response_body) 
    response = response_body
    loaded=true
end
