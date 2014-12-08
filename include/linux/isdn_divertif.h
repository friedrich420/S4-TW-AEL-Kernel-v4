         toIndex = Math.min(toIndex, oldLength);
        var newIndex = Math.min(fromIndex, oldLength);
        for (var i = newIndex; i < toIndex; ++i) {
            var call = this._calls[i];
            if (functionNames.indexOf(call.functionName()) === -1)
                this._calls[newIndex++] = call;
        }
        if (newIndex >= toIndex)
            return false;
        this._calls.splice(newIndex, toIndex - newIndex);
        return true;
    },

    /**
     * @return {!Object.<string, string>}
     */
    _currentAttributesState: function()
    {
        var ctx = this.wrappedObject();
        var state = {};
        state.attributes = {};
        CanvasRenderingContext2DResource.AttributeProperties.forEach(function(attribute) {
            state.attributes[attribute] = ctx[attribute];
        });
        if (ctx.getLineDash)
            state.lineDash = ctx.getLineDash();
        return state;
    },

    /**
     * @param {Object.<string, string>=} state
     */
    _applyAttributesState: function(state)
    {
        if (!state)
            return;
        var ctx = this.wrappedObject();
        if (state.attributes) {
            Object.keys(state.attributes).forEach(function(attribute) {
                ctx[attribute] = state.attributes[attribute];
            });
        }
        if (ctx.setLineDash)
            ctx.setLineDash(state.lineDash);
    },

    /**
     * @override
     * @return {!Object.<string, Function>}
     */
    _customWrapFunctions: function()
    {
        var wrapFunctions = CanvasRenderingContext2DResource._wrapFunctions;
        if (!wrapFunctions) {
            wrapFunctions = Object.create(null);

            wrapFunctions["createLinearGradient"] = Resource.WrapFunction.resourceFactoryMethod(LogEverythingResource, "CanvasGradient");
            wrapFunctions["createRadialGradient"] = Resource.WrapFunction.resourceFactoryMethod(LogEverythingResource, "CanvasGradien